/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef SHAREDMEMORYHOLDERPRIVATE_H
#define SHAREDMEMORYHOLDERPRIVATE_H

#include <cstddef>

namespace cf {

class SharedMemoryHolderPrivate
{
public:
    enum error_t {
        NO_ERROR = 0,
        UNKNOWN,
        /* A shared memory segment is already associated with key
            and the caller has no permission to access it */
        NO_ACCESS,
        /* no shared memory segment associated with key was found. */
        NOT_FOUND,
        /* There is not enough memory left to created a shared memory segment of the requested size. */
        NO_MEMORY,
        /* A new shared memory identifier could not be created
            because the system limit for the number of shared memory identifiers has been reached. */
        LIMITS,
        /* given address is not the start address of a mapped shared memory segment */
        BAD_ADDRESS
    };
public:
    virtual ~SharedMemoryHolderPrivate() {}
    virtual void close(void * mem) = 0;
    virtual void * create(size_t key, size_t size) = 0;
    virtual void * open(size_t key, size_t size) = 0;
    virtual bool remove() = 0;
    virtual error_t error() const { return NO_ERROR; }
    virtual size_t limit() const = 0;
};

}

#endif // SHAREDMEMORYHOLDERPRIVATE_H
