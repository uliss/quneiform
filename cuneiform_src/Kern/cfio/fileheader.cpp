/*
 * fileheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "fileheader.h"
#include "ctc_def.h"

namespace CIF {
namespace CTC {

FileHeader::FileHeader() :
	GlobalHeader() {
	pFile = NULL;
}

FileHeader::~FileHeader() {
	if (GetFile()) {
		delete GetFile();
	}
}

FileHeader::FileHeader(GlobalFile * pNewFile, uint Flag, Handle Storage) :
	GlobalHeader(NULL, NULL, 0) {
	pFile = pNewFile;
	SetHandle(AcceptFile(pNewFile));
	SetFlag(Flag);
	SetHeaderSize(sizeof(class FileHeader));
	AttachToStorage(Storage);
	BreakName();
}

bool FileHeader::DetachFromStorage() {
	return ((hStorage = NULL) == NULL);
}

bool FileHeader::AttachToStorage(Handle Storage) {
	hStorage = Storage;
	return true;
}

bool FileHeader::LockToStorage() {
	if (CanWrite() && GetAttaching()) {
		return AddFlag(CFIO_FILE_LOCKED);
	}
	return false;
}

bool FileHeader::UnlockFromStorage() {
	if (!CanWrite() && GetAttaching()) {
		return RemoveFlag(CFIO_FILE_LOCKED);
	}
	return false;
}
}
}
