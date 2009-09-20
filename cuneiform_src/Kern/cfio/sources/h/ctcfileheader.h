/*
 * ctcfileheader.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CTCFILEHEADER_H_
#define CTCFILEHEADER_H_

#include "ctc_def.h"
#include "ctcglobalheader.h"
#include "ctcglobalfile.h"

namespace CIF {
namespace CTC {

class FileHeader: public GlobalHeader {
private:
	GlobalFile * pFile;
	Handle hStorage;
	uint32_t wFlag;
	Bool32 KeepFileName;

public:
	Bool32 UnlockFromStorage(void);
	Bool32 LockToStorage(void);
	FileHeader();
	FileHeader(GlobalFile * pNewFile, uint32_t Flag = CFIO_FILE_READ
			| CFIO_FILE_WRITE, Handle hStorage = NULL);
	~FileHeader();

public:
	Bool32 AttachToStorage(Handle Storage);
	Bool32 DetachFromStorage();
	FileHeader * GetNext(void) {
		return (FileHeader *) (GlobalHeader::GetNext());
	}

	GlobalFile * GetFile(void) {
		return pFile;
	}

	Handle GetAttaching(void) {
		return hStorage;
	}

	Bool32 CanWrite(void) {
		return !IsFlag(CFIO_FILE_LOCKED);
	}

	Bool32 KeepName(void) {
		return KeepFileName = TRUE;
	}

	Bool32 BreakName(void) {
		return !(KeepFileName = FALSE);
	}

	Bool32 HowName(void) {
		return KeepFileName;
	}

private:
	Handle AcceptFile(GlobalFile * File) {
		return (pFile = File)->GetFileHandle();
	}
};

}
}

#endif /* CTCFILEHEADER_H_ */
