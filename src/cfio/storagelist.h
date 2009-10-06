/*
 * storagelist.h
 *
 *  Created on: 21.09.2009
 *      Author: uliss
 */

#ifndef CIF_CTC_STORAGELIST_H_
#define CIF_CTC_STORAGELIST_H_

#include "storageheader.h"

namespace CIF {
namespace CFIO {

class GlobalFile;

typedef struct {
	size_t siHeaderSize;
	size_t siItemSize;
	size_t siFlag;
	char siName[CFIO_MAX_PATH];
} STORAGEITEM;

class StorageList {
public:
	StorageList();
	~StorageList();

	Handle FindStorage(const std::string& Name);
	Handle AddItem(GlobalFile * NewStorage, uint32_t Parametr);
	bool DeleteItem(Handle Storage);
	GlobalFile * GetItem(Handle Storage);
	StorageHeader * GetFirstItemHeader() {
		return pFirst()->GetNext();
	}

	StorageHeader * GetItemHeader(Handle Storage);
	bool IsEmpty() const {
		return item_counter_ == 0;
	}

private:
	StorageHeader * pFirst() {
		return &first_;
	}

	StorageHeader * pLast() {
		return &last_;
	}

	size_t IncreaseItemCounter() {
		return ++item_counter_;
	}

	size_t DecreaseItemCounter() {
		return --item_counter_;
	}

	size_t HowItems() const {
		return item_counter_;
	}

private:
	size_t header_size_;
	size_t item_counter_;
	StorageHeader first_;
	StorageHeader last_;
	StorageHeader * list_;
};

}
}

#endif /* CIF_CTC_STORAGELIST_H_ */
