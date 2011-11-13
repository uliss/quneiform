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
    virtual ~SharedMemoryHolderPrivate() {};
    virtual void close(void * mem) = 0;
    virtual void * create(int key, size_t size) = 0;
    virtual void * open(int key, size_t size) = 0;
    virtual bool remove() = 0;
};

}

#endif // SHAREDMEMORYHOLDERPRIVATE_H
