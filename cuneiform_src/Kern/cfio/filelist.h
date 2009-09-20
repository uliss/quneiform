/*
 * filelist.h
 *
 *  Created on: 21.09.2009
 *      Author: uliss
 */

#ifndef CIF_CTC_FILELIST_H_
#define CIF_CTC_FILELIST_H_

#include "cttypes.h"
#include "ctcfileheader.h"
#include "ctcglobalfile.h"

namespace CIF {
namespace CTC {

class StorageHeader;

class FileList {
public:
	Handle FindFile(const std::string& FileName);
	FileList();
	~FileList();

	Handle AddItem(GlobalFile * NewFile, uint NewFlag, Handle Storage);
	bool DeleteItem(Handle File);
	Handle GetAttachedFileHeader(Handle Storage, FileHeader * File = NULL);
	FileHeader * GetItemHeader(Handle File);
	GlobalFile * GetItem(Handle File);

private:
	FileHeader * pLast() {
		return &last_;
	}

	FileHeader * pFirst() {
		return &first_;
	}

	uint32_t IncreaseFileCounter() {
		return ++file_counter_;
	}
	uint32_t DecreaseFileCounter() {
		return --file_counter_;
	}
	uint32_t IncreaseSpaceCounter() {
		return ++space_sounter_;
	}
	uint32_t DecreaseSpaceCounter() {
		return --space_sounter_;
	}

private:
	size_t list_size_;
	StorageHeader * list_;
	size_t file_counter_;
	size_t space_sounter_;
	FileHeader first_;
	FileHeader last_;

};

}
}

#endif /* CIF_CTC_FILELIST_H_ */
