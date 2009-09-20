/*
 * ctcmemoryheader.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CTCMEMORYHEADER_H_
#define CTCMEMORYHEADER_H_

#include <string>
#include "ctcglobalheader.h"

namespace CIF {
namespace CTC {

#define                 CFIO_MEMORY_GLOBAL           0x0001
#define                 CFIO_MEMORY_LOCK             0x0002
#define                 CFIO_MEMORY_UNUSED           0x0004
#define                 CFIO_MEMORY_FREE             0x0008

class MemoryHeader: public GlobalHeader {
public:
	MemoryHeader();
	MemoryHeader(Handle hMemory, uint32_t wBlockSize);
	MemoryHeader(Handle hMemory, uint32_t wBlockSize, const std::string& OwnerName, const std::string& Comment);
	~MemoryHeader();

	std::string GetOwner() const {
		return owner_;
	}

	std::string GetComment() const {
		return comment_;
	}

	MemoryHeader * GetNext() {
		return static_cast<MemoryHeader*>(GlobalHeader::GetNext());
	}
private:
	std::string comment_, owner_;
};

typedef MemoryHeader *  MemoryHeaderPtr;
typedef MemoryHeader ** PPMemoryHeader;

}
}

#endif /* CTCMEMORYHEADER_H_ */
