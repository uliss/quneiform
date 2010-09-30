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

#include "ctcmemoryheader.h"
#include "ctcclasses.h"

namespace cf
{
namespace CTC
{

MemoryHeader::MemoryHeader() :
        GlobalHeader()
{
}

MemoryHeader::MemoryHeader(Handle hMemory, uint32_t wBlockSize) :
        GlobalHeader(hMemory, NULL, wBlockSize)
{
    SetHeaderSize(sizeof(class MemoryHeader));
}

MemoryHeader::MemoryHeader(Handle hMemory, uint32_t wBlockSize,
                           const std::string& OwnerName, const std::string& Commentary) :
        GlobalHeader(hMemory, NULL, wBlockSize), comment_(Commentary), owner_(
            OwnerName)
{
    SetHeaderSize(sizeof(class MemoryHeader));
}

MemoryHeader::~MemoryHeader()
{
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
