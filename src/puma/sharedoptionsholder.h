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

#ifndef SHAREDOPTIONSHOLDER_H
#define SHAREDOPTIONSHOLDER_H

#include "shared_memory_type.h"
#include "globus.h"

namespace cf
{

class SharedOptions;

class CLA_EXPO SharedOptionsHolder
{
public:
    /**
      * Constructs SharedOptions in shared memory
      */
    SharedOptionsHolder(SharedMemory * segment);

    /**
      * Removes SharedOptions from shared memory
      */
    ~SharedOptionsHolder();

    /**
      * Returns pointer to shared options or NULL if not found
      */
    SharedOptions * options();
public:
    /**
      * Finds shared options in given memory segment
      * @return pointer to shred result or NULL if not found
      */
    static SharedOptions * find(SharedMemory * segment);
private:
    SharedMemory * segment_;
    SharedOptions * options_;
};
}

#endif // SHAREDOPTIONSHOLDER_H
