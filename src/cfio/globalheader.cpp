/*
 * globalheader.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "globalheader.h"

namespace CIF {

namespace CFIO {

GlobalHeader::GlobalHeader(Handle NewHandle, void * NewData, uint NewSize,
		uint NewFlag, GlobalHeader * NewNext) {
	SetHandle(NewHandle);
	SetData(NewData);
	SetNext(NewNext);
	SetSize(NewSize);
	SetFlag(NewFlag);
}
}

}
