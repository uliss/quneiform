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

#ifndef SHAREDRESULTHOLDER_H
#define SHAREDRESULTHOLDER_H

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif
#include <boost/interprocess/managed_shared_memory.hpp>

#include "globus.h"
#include "ced/cedpageptr.h"

namespace bi = boost::interprocess;

namespace cf
{

class SharedResult;

class CLA_EXPO SharedResultHolder
{
public:
    /**
      * Constructs SharedResult in shared memory
      */
    SharedResultHolder(bi::managed_shared_memory * segment);

    /**
      * Removes SharedResult from shared memory
      */
    ~SharedResultHolder();

    /**
      * Returns page stored in shared memory
      * if fails - returns empty pointer
      */
    CEDPagePtr page();

    /**
      * Returns pointer to shared result
      */
    SharedResult * sharedResult();
public:
    /**
      * Finds shared result in given memory segment
      * @return pointer to shred result or NULL if not found
      */
    static SharedResult * find(bi::managed_shared_memory * segment);
private:
    bi::managed_shared_memory * segment_;
    SharedResult * shm_result_;
};
}

#endif // SHAREDRESULTHOLDER_H
