/*
 * ctcfileheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "ctcfileheader.h"
#include "ctc_def.h"

namespace CIF {
namespace CTC {

//////////////////////////////////////////////////////////////////////////////////
//
FileHeader::FileHeader() :
	GlobalHeader() {
	pFile = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
FileHeader::~FileHeader() {
	if (GetFile()) {
		delete GetFile();
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
FileHeader::FileHeader(GlobalFile * pNewFile, uint32_t Flag,
		Handle Storage) :
	GlobalHeader(NULL, NULL, 0) {
	pFile = pNewFile;
	SetHandle(AcceptFile(pNewFile));
	SetFlag(Flag);
	SetHeaderSize(sizeof(class FileHeader));
	AttachToStorage(Storage);
	BreakName();
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 FileHeader::DetachFromStorage() {
	return ((hStorage = NULL) == NULL);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 FileHeader::AttachToStorage(Handle Storage) {
	if (TRUE /*!IsFlag(CFIO_FILE_LOCKED)*/) {
		hStorage = Storage;
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 FileHeader::LockToStorage(void) {
	if (CanWrite() && GetAttaching()) {
		return AddFlag(CFIO_FILE_LOCKED);
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 FileHeader::UnlockFromStorage(void) {
	if (!CanWrite() && GetAttaching()) {
		return RemoveFlag(CFIO_FILE_LOCKED);
	}
	return FALSE;
}
}
}
