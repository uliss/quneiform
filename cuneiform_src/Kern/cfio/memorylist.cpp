/*
 * memorylist.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "memorylist.h"
#include "resource.h"

extern void SetReturnCode_cfio(uint16_t rc);

namespace CIF {
namespace CTC {

MemoryList::MemoryList() :
	first_(FICTIV_Handle, 0, "CFIO", "First in list (fictiv)"),
			last_(FICTIV_Handle, 0, "CFIO", "Last in list (fictiv)") {
	first_.SetNext(&last_);
	list_size_ = sizeof(class MemoryList);
	memory_counter_ = 0;
	item_counter_ = 0;
}

MemoryList::~MemoryList() {
	while (pFirst()->GetNext() != pLast()) {
#ifdef TRACE
		TRACE("CFIO: Missed memory block { handle: 0x%x, data: 0x%x, size: %i, owner:\"%s\", comment:\"%s\" } allocated by CFIO \n",
				pFirst()->GetNext()->GetHandle(),
				pFirst()->GetNext()->GetData(),
				pFirst()->GetNext()->GetSize(),
				pFirst()->GetNext()->GetOwner(),
				pFirst()->GetNext()->GetComment());
#endif
		KillItem(NULL, pFirst());
	}
}

size_t MemoryList::IncreaseMemoryCounter(size_t wSize) {
	return (memory_counter_ = memory_counter_ + wSize);
}

size_t MemoryList::DecreaseMemoryCounter(size_t wSize) {
	return (memory_counter_ = memory_counter_ - wSize);
}

MemoryHeader * MemoryList::pFirst() {
	return &first_;
}

MemoryHeader * MemoryList::pLast() {
	return &last_;
}

bool MemoryList::AddItem(Handle hMemory, size_t Size, uint32_t wIsGlobal,
		const std::string& Owner, const std::string& Comment) {
	MemoryHeader * Current;
	for (Current = pFirst(); Current->GetNext() != pLast(); Current
			= Current->GetNext()) {
		if (Current->GetHandle() == hMemory)
			return false;
	}

	MemoryHeader * NewBlock = new MemoryHeader(hMemory, Size, Owner, Comment);
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock);
	IncreaseMemoryCounter(NewBlock->GetSize());

	if (wIsGlobal) {
		NewBlock->AddFlag(CFIO_MEMORY_GLOBAL);
	} else {
		NewBlock->SetData(hMemory);
	}

	return true;
}

bool MemoryList::DeleteItem(Handle hMemory) {
	MemoryHeaderPtr Current, Last;
	size_t IsOK = 0;

	for (Last = Current = pFirst(); Current != pLast(); Current
			= Current->GetNext()) {
		if (Current->GetHandle() == hMemory) {
			if (KillItem(Current, Last))
				IsOK++;
			Current = Last;
		} else {
			Last = Current;
		}
	}
	return (IsOK == 1);
}

bool MemoryList::KillItem(MemoryHeader * pItem, MemoryHeader * pPrevItem) {
	MemoryHeader * pErased = (pItem) ? pItem : pPrevItem->GetNext();

	DecreaseMemoryCounter(pErased->GetSize());
	pPrevItem->SetNext(pErased->GetNext());
	delete pErased;
	return true;
}

MemoryHeader * MemoryList::GetItem(Handle hMemory) {
	for (MemoryHeader * Current = pFirst(); Current != pLast(); Current
			= Current->GetNext()) {
		if (!Current) {
			SetReturnCode_cfio(IDS_CFIO_ERR_INTERNAL);
			return NULL;
		}

		if (Current->GetHandle() == hMemory)
			return Current;
	}

	return NULL;
}

bool MemoryList::TakeItem(Handle hMemory, size_t * Size, uint * Flag) {
	MemoryHeader * Current = GetItem(hMemory);

	if (Current) {
		*Size = Current->GetSize();
		*Flag = Current->GetFlag();
		return true;
	}

	*Size = 0;
	*Flag = 0;
	return false;
}

// Add and remove Lock Flag (depend by bLock)
bool MemoryList::LockUnlockItem(Handle hMemory, bool Lock) {
	for (MemoryHeader * Current = pFirst(); Current != pLast(); Current
			= Current->GetNext()) {
		if (Current->GetHandle() == hMemory) {
			if (Lock && !Current->IsFlag(CFIO_MEMORY_LOCK)) {
				Current->AddFlag(CFIO_MEMORY_LOCK);
				return true;
			}

			if (!Lock && Current->IsFlag(CFIO_MEMORY_LOCK)) {
				Current->RemoveFlag(CFIO_MEMORY_LOCK);
				return true;
			}
		}
	}
	return false;
}
}
}

