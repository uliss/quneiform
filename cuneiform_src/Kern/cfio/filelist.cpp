/*
 * filelist.cpp
 *
 *  Created on: 21.09.2009
 *      Author: uliss
 */

#include "filelist.h"
#include "storageheader.h"

namespace CIF {
namespace CFIO {

FileList::FileList() {
	first_.SetNext(&last_);
	first_.SetSize(0);
	last_.SetSize(0);
	first_.SetHandle(FICTIV_Handle);
	last_.SetHandle(FICTIV_Handle);

	list_ = NULL;
	file_counter_ = 0;
	space_sounter_ = 0;
}

FileList::~FileList() {

}

Handle FileList::AddItem(GlobalFile * NewFile, uint NewFlag, Handle Storage) {
	FileHeader * Current, *NewBlock = NULL;
	Handle NewHandle = NewFile->GetFileHandle();

	if (!NewHandle)
		return NULL;

	for (Current = pFirst(); Current->GetNext() != pLast(); Current
			= Current->GetNext())
		if (Current->GetHandle() == NewFile->GetFileHandle()) {
			return NULL;
		}

	NewBlock = new FileHeader(NewFile, NewFlag, Storage);
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock);
	IncreaseFileCounter();

	return NewHandle;
}

bool FileList::DeleteItem(Handle File) {
	FileHeader * Current, *Last, *EraseBlock;
	size_t IsOK = 0;

	for (Last = Current = pFirst(); Current != pLast(); Current
			= Current->GetNext()) {
		if (Current->GetHandle() == File) {
			EraseBlock = Current;
			DecreaseFileCounter();
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

GlobalFile * FileList::GetItem(Handle File) {
	FileHeader * pCurrent = GetItemHeader(File);
	GlobalFile * pFounded = NULL;

	if (pCurrent) {
		pFounded = pCurrent->GetFile();
	}

	return pFounded;
}

FileHeader * FileList::GetItemHeader(Handle File) {
	FileHeader * pCurrent;

	for (pCurrent = pFirst(); pCurrent != pLast(); pCurrent
			= pCurrent->GetNext()) {
		if (pCurrent->GetHandle() == File) {
			return pCurrent;
		}
	}

	return NULL;
}

Handle FileList::GetAttachedFileHeader(Handle Storage, FileHeader * File) {
	FileHeader * pCurrent, *pStart;

	if (File == NULL) {
		pStart = pFirst();
	} else {
		pStart = File;

		if (!pStart)
			return NULL;
	}

	for (pCurrent = pStart; pCurrent != pLast(); pCurrent = pCurrent->GetNext()) {
		if (pCurrent->GetAttaching() == Storage) {
			return pCurrent->GetHandle();
		}
	}
	return NULL;
}

Handle FileList::FindFile(const std::string& FileName) {
	for (FileHeader * pCurrent = pFirst(); pCurrent != pLast(); pCurrent
			= pCurrent->GetNext()) {
		GlobalFile * pFile = pCurrent->GetFile();

		if (pFile && (pFile->GetFileName() == FileName))
			return pCurrent->GetHandle();
	}
	return NULL;
}
}
}
