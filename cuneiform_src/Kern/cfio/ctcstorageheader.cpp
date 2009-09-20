/*
 * ctcstorageheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include <cstring>

#include "ctcstorageheader.h"
#include "ctcglobalfile.h"
#include "ctccontrol.h"
#include "compat_defs.h"
#include "cfio.h"

namespace CIF {
namespace CTC {

static char ShFolder[CFIO_MAX_PATH];
static char ShFile[CFIO_MAX_PATH + 4];
static char ShExtension[CFIO_MAX_PATH];
static char ShBuffer[CFIO_MAX_PATH + 4];

StorageHeader::StorageHeader() :
	GlobalHeader() {
	pStorageFile = NULL;
	wContensCounter = 0;
}

StorageHeader::StorageHeader(GlobalFile * pNewStorage, uint32_t wNewFlag,
		const std::string& NewStorageFolder) :
	GlobalHeader(pNewStorage, NULL, 0, wNewFlag) {
	extern CTCControl * Control_ctc;

	SetHandle(AcceptFile(pNewStorage));
	SetFlag(wNewFlag);
	SetHeaderSize(sizeof(class FileHeader));

	if (!NewStorageFolder.empty()) {
		CFIO_GETFOLDERSITEMS(NewStorageFolder.c_str(), ShFolder, ShFile,
				ShExtension);
		// берем временную директорию
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		// отписываем туда
		CFIO_MAKEPATH(ShBuffer, ShFolder, ShFile, ShExtension);
		folder_ = ShBuffer;
	} else {
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		CFIO_MAKEFOLDER(ShFolder);

		if (GetTempFileName(ShFolder, "STG", 0, ShFile)) {
			unlink(ShFile);
			CFIO_STRCPY(ShBuffer, ShFile);
			CFIO_GETFOLDERSITEMS(ShBuffer, ShFolder, ShFile, ShExtension);
			CFIO_MAKEPATH(ShBuffer, ShFolder, ShFile, NULL);
			folder_ = ShBuffer;
		} else {
#ifdef _DEBUG
			uint32_t Err = GetLastError();
			pvoid lpMsgBuf;

			FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					Err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(char*) &lpMsgBuf,
					0,
					NULL
			);

			fprintf(stderr, ( char* ) lpMsgBuf, "CFIO: Storage can't create own unpack folder");
			LocalFree( lpMsgBuf );
			Control_ctc->GetFolder(CFIO_STORAGE_FOLDER, ShFile);
#endif
			folder_ = ShFile;
		}
	}
}

StorageHeader::~StorageHeader() {
	if (GetStorage()) {
		delete GetStorage();
	}

}

Handle StorageHeader::AcceptFile(GlobalFile * File) {
	pStorageFile = File;
	return pStorageFile->GetFileHandle();
}
}
}
