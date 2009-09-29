/*
 * storageheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include <cstring>
#include <climits>

#include "storageheader.h"
#include "globalfile.h"
#include "control.h"
#include "compat_defs.h"
#include "cfio.h"

namespace CIF {
namespace CFIO {

static char ShFolder[PATH_MAX];
static char ShFile[PATH_MAX + 4];
static char ShExtension[PATH_MAX];
static char ShBuffer[PATH_MAX + 4];

StorageHeader::StorageHeader() :
	GlobalHeader(), contents_counter_(0) {
	storage_file_ = NULL;
}

StorageHeader::StorageHeader(GlobalFile * pNewStorage, uint NewFlag,
		const std::string& NewStorageFolder) :
	GlobalHeader(pNewStorage, NULL, 0, NewFlag), contents_counter_(0) {
	extern Control * Control_ctc;

	SetHandle(AcceptFile(pNewStorage));
	SetFlag(NewFlag);
	SetHeaderSize(sizeof(class FileHeader));

	if (!NewStorageFolder.empty()) {
		CFIO_GETFOLDERSITEMS(NewStorageFolder.c_str(), ShFolder, ShFile,
				ShExtension);
		// берем временную директорию
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		// отписываем туда
		folder_ = CIF::MakePath(ShFolder, ShFile, ShExtension);
	} else {
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		CFIO_MAKEFOLDER(ShFolder);

		if (GetTempFileName(ShFolder, "STG", 0, ShFile)) {
			unlink(ShFile);
			CFIO_STRCPY(ShBuffer, ShFile);
			CFIO_GETFOLDERSITEMS(ShBuffer, ShFolder, ShFile, ShExtension);
			folder_ = CIF::MakePath(ShFolder, ShFile, ShExtension);
		} else {
			folder_ = ShFile;
		}
	}
}

StorageHeader::~StorageHeader() {
	if (GetStorage())
		delete GetStorage();
}

Handle StorageHeader::AcceptFile(GlobalFile * File) {
	storage_file_ = File;
	return storage_file_->GetFileHandle();
}
}
}
