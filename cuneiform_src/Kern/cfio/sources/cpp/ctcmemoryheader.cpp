/*
 * ctcmemoryheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "ctcmemoryheader.h"
#include "ctc_def.h"

namespace CIF {
namespace CTC {

MemoryHeader::MemoryHeader() :
	GlobalHeader() {
}

MemoryHeader::MemoryHeader(Handle hMemory, uint32_t wBlockSize) :
	GlobalHeader(hMemory, NULL, wBlockSize) {
	SetHeaderSize(sizeof(class MemoryHeader));
}

MemoryHeader::MemoryHeader(Handle hMemory, uint32_t wBlockSize,
		const std::string& OwnerName, const std::string& Commentary) :
	GlobalHeader(hMemory, NULL, wBlockSize), comment_(Commentary), owner_(
			OwnerName) {
	SetHeaderSize(sizeof(class MemoryHeader));
}

MemoryHeader::~MemoryHeader() {
	Handle hToDelete = GetHandle();

	if (hToDelete != NULL && hToDelete != FICTIV_Handle) {
#ifdef CFIO_USE_GLOBAL_MEMORY
		CFIO_FREE(GetHandle());
#else
		delete[] static_cast<char*> (hToDelete);
#endif //CFIO_USE_GLOBAL_MEMORY
	}
}
}
}
