/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "linesbuffer.h"
#include "cfio/cfio.h"

namespace CIF {

using namespace CFIO;

LinesBuffer::LinesBuffer() :
    LineInfoA(NULL), LineInfoB(NULL), HLinesBufferA(NULL), VLinefBufferA(NULL), HLinesBufferB(0),
            VLinefBufferB(0) {

}

void LinesBuffer::alloc() {
    LineInfoA = static_cast<LinesTotalInfo*> (CFIO_DAllocMemory(sizeof(LinesTotalInfo),
            CFIO::MAF_GALL_GPTR, "puma", "SVL step I lines info pool"));
    LineInfoB = static_cast<LinesTotalInfo*> (CFIO_DAllocMemory(sizeof(LinesTotalInfo),
            CFIO::MAF_GALL_GPTR, "puma", "SVL step II lines info pool"));
}

void LinesBuffer::free() {
    CFIO_FreeMemory(LineInfoA);
    CFIO_FreeMemory(LineInfoB);
    LineInfoA = NULL;
    LineInfoB = NULL;

    if (VLinefBufferA != NULL)
        CFIO_FreeMemory(VLinefBufferA);

    if (VLinefBufferB != NULL)
        CFIO_FreeMemory(VLinefBufferB);

    VLinefBufferA = NULL;
    VLinefBufferB = NULL;
}

}
