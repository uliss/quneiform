/*
 * memorylist.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef MEMORYLIST_H_
#define MEMORYLIST_H_

#include "cttypes.h"
#include "ctc_def.h"
#include "ctcmemoryheader.h"

#include <string>

namespace CIF {
namespace CTC {

class MemoryList {
public:
	MemoryList();
	~MemoryList();

	bool AddItem(Handle hMemory, size_t Size, uint32_t IsGlobal,
			const std::string& Owner, const std::string& Coment);
	bool DeleteItem(Handle hMemory);
	MemoryHeaderPtr GetItem(Handle hMemory);
	bool LockUnlockItem(Handle hMemory, bool Lock);
	bool TakeItem(Handle hMemory, size_t * Size, uint * Flag);
private:
	bool KillItem(MemoryHeaderPtr Item, MemoryHeaderPtr PrevItem);
	MemoryHeaderPtr pFirst();
	MemoryHeaderPtr pLast();
	size_t IncreaseMemoryCounter(uint32_t wSize);
	size_t DecreaseMemoryCounter(uint32_t wSize);

	size_t list_size_;
	MemoryHeader first_;
	MemoryHeader last_;
	size_t memory_counter_;
	size_t item_counter_;
};

}
}

#endif /* MEMORYLIST_H_ */
