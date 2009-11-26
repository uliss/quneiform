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

#ifndef CTCMEMORYHEADER_H_
#define CTCMEMORYHEADER_H_

#include <string>
#include "ctcglobalheader.h"

namespace CIF
{
namespace CTC
{

#define                 CFIO_MEMORY_GLOBAL           0x0001
#define                 CFIO_MEMORY_LOCK             0x0002
#define                 CFIO_MEMORY_UNUSED           0x0004
#define                 CFIO_MEMORY_FREE             0x0008

class MemoryHeader: public GlobalHeader
{
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
