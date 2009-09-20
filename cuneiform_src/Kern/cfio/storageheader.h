/*
 * ctcstorageheader.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CTCSTORAGEHEADER_H_
#define CTCSTORAGEHEADER_H_

#include <string>

#include "ctcglobalheader.h"
#include "cfio.h"
#include "ctc_def.h"

namespace CIF {
namespace CTC {

class GlobalFile;

class StorageHeader: public GlobalHeader {
private:
	GlobalFile * storage_file_;
	int contents_counter_;
	std::string name_;
	std::string folder_;

public:
	StorageHeader();
	StorageHeader(GlobalFile * pNewStorage, uint Flag = CFIO_FILE_READ
			| CFIO_FILE_WRITE, const std::string& NewStorageFolder =
			std::string());
	~StorageHeader();

	StorageHeader * GetNext(void) {
		return static_cast<StorageHeader*> (GlobalHeader::GetNext());
	}

	GlobalFile * GetStorageFile(void) {
		return storage_file_;
	}

	int IncreaseContentsCounter() {
		return ++contents_counter_;
	}

	int DecreaseContentsCounter() {
		return --contents_counter_;
	}

	GlobalFile * GetStorage() {
		return storage_file_;
	}

	std::string GetStorageFolder() const {
		return folder_;
	}

private:
	Handle AcceptFile(GlobalFile * File);
};
}
}

#endif /* CTCSTORAGEHEADER_H_ */
