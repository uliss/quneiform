/*
 * ctcglobalheader.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CTCGLOBALHEADER_H_
#define CTCGLOBALHEADER_H_

#include <cstddef>
#include "cttypes.h"

namespace CIF {

namespace CTC {

class GlobalHeader {
public:
	GlobalHeader() {}
	GlobalHeader(Handle NewHandle, void * NewData = NULL, uint NewSize = 0,
			uint NewFlag = 0, GlobalHeader * NewNext = NULL);

	~GlobalHeader() {}

	Handle GetHandle() {
		return hGlobalHandle;
	}

	void * GetData() {
		return static_cast<void *> (pcMemoryBlock);
	}

	uint GetFlag() const {
		return wFlag;
	}

	uint GetSize() const {
		return wSize;
	}

	uint GetHeaderSize() const {
		return wHeaderSize;
	}

	GlobalHeader * GetNext() {
		return pNext;
	}

	Handle SetHandle(Handle GlobalHandle) {
		return (hGlobalHandle = GlobalHandle);
	}

	void * SetData(void * Data) {
		return static_cast<void*> (pcMemoryBlock = static_cast<char*> (Data));
	}

	GlobalHeader * SetNext(GlobalHeader * Next) {
		return (pNext = Next);
	}

	uint SetSize(uint Size) {
		return (wSize = Size);
	}

	uint SetHeaderSize(uint Size) {
		return (wHeaderSize = Size);
	}

	uint SetFlag(uint Flag) {
		return (wFlag = Flag);
	}

	bool IsFlag(uint Flag) const {
		return (wFlag & Flag);
	}

	bool AddFlag(uint Flag) {
		return (IsFlag(wFlag |= Flag));
	}

	bool RemoveFlag(uint Flag) {
		return (!IsFlag(wFlag &= ~Flag));
	}
protected:
	Handle hGlobalHandle;
	char * pcMemoryBlock;
	GlobalHeader * pNext;
	uint wSize;
	uint wHeaderSize;
	uint wFlag;
};

}
}

#endif /* CTCGLOBALHEADER_H_ */
