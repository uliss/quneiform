/*
 * fileheader.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CIF_CTC_FILEHEADER_H_
#define CIF_CTC_FILEHEADER_H_

#include "ctc_def.h"
#include "globalheader.h"
#include "globalfile.h"

namespace CIF {
namespace CTC {

class FileHeader: public GlobalHeader {
private:
	GlobalFile * pFile;
	Handle hStorage;
	uint wFlag;
	bool KeepFileName;

public:
	FileHeader();
	FileHeader(GlobalFile * pNewFile, uint Flag = CFIO_FILE_READ
			| CFIO_FILE_WRITE, Handle hStorage = NULL);
	~FileHeader();

	bool AttachToStorage(Handle Storage);
	bool DetachFromStorage();

	FileHeader * GetNext() {
		return static_cast<FileHeader*> (GlobalHeader::GetNext());
	}

	GlobalFile * GetFile() {
		return pFile;
	}

	Handle GetAttaching() {
		return hStorage;
	}

	bool CanWrite() {
		return !IsFlag(CFIO_FILE_LOCKED);
	}

	bool KeepName() {
		return KeepFileName = true;
	}

	bool BreakName() {
		return !(KeepFileName = false);
	}

	bool HowName() const {
		return KeepFileName;
	}

	bool LockToStorage();
	bool UnlockFromStorage();
private:
	Handle AcceptFile(GlobalFile * File) {
		return (pFile = File)->GetFileHandle();
	}
};

}
}

#endif /* CIF_CTC_FILEHEADER_H_ */
