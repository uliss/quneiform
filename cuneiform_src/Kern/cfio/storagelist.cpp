/*
 * storagelist.cpp
 *
 *  Created on: 21.09.2009
 *      Author: uliss
 */

#include "storagelist.h"
#include "globalfile.h"

namespace CIF {
namespace CFIO {

StorageList::StorageList() {
	first_.SetNext(&last_);
	first_.SetSize(0);
	last_.SetSize(0);
	first_.SetHandle(FICTIV_Handle);
	last_.SetHandle(FICTIV_Handle);

	list_ = NULL;
	item_counter_ = 0;
}

StorageList::~StorageList() {
}

Handle StorageList::AddItem(GlobalFile * NewStorage, uint NewFlag) {
	StorageHeader * Current, *NewBlock = NULL;
	Handle NewHandle = NewStorage->GetFileHandle();

	if (!NewHandle)
		return NULL;

	for (Current = pFirst(); Current->GetNext() != pLast(); Current
			= Current->GetNext())
		if (Current->GetHandle() == NewStorage->GetFileHandle()) {
			return NULL;
		}

	NewBlock = new StorageHeader(NewStorage, NewFlag, "");
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock);
	IncreaseItemCounter();

	return NewHandle;
}

bool StorageList::DeleteItem(Handle Storage) {
	StorageHeader * Current, *Last, *EraseBlock;
	size_t IsOK = 0;

	for (Last = Current = pFirst(); Current != pLast(); Current
			= Current->GetNext()) {
		if (Current->GetHandle() == Storage) {
			EraseBlock = Current;
			DecreaseItemCounter();
			Last->SetNext(Current->GetNext());
			delete EraseBlock;
			IsOK++;
			Current = Last;
		} else {
			Last = Current;
		}
	}
	return (IsOK == 1);
}

GlobalFile * StorageList::GetItem(Handle Storage) {
	StorageHeader * pCurrent = GetItemHeader(Storage);
	return pCurrent ?  pCurrent->GetStorageFile() : NULL;
}

StorageHeader * StorageList::GetItemHeader(Handle Storage) {
	for (StorageHeader * pCurrent = pFirst(); pCurrent != pLast(); pCurrent
			= pCurrent->GetNext()) {
		if (pCurrent->GetHandle() == Storage) {
			return pCurrent;
		}
	}
	return NULL;
}

Handle StorageList::FindStorage(const std::string& StorageName) {
	for (StorageHeader * pCurrent = pFirst(); pCurrent != pLast(); pCurrent
			= pCurrent->GetNext()) {
		GlobalFile * pStorage = pCurrent->GetStorage();

		if (pStorage && (pStorage->GetFileName() == StorageName))
			return pCurrent->GetHandle();
	}
	return NULL;

}
}
}
