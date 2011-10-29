/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef SHAREDIMAGEHOLDER_H
#define SHAREDIMAGEHOLDER_H

#include "shared_memory_type.h"
#include "globus.h"

namespace cf {

class SharedImage;

class CLA_EXPO SharedImageHolder
{
public:
    SharedImageHolder(SharedMemory * segment);
    ~SharedImageHolder();
    SharedImage * image();
public:
    static SharedImage * find(SharedMemory * segment);
private:
    SharedMemory * segment_;
    SharedImage * image_;
};

}

#endif // SHAREDIMAGEHOLDER_H
