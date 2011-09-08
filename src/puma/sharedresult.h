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

#ifndef SHAREDRESULT_H
#define SHAREDRESULT_H

#include <cstddef>

#include "globus.h"
#include "ced/cedpage.h"

namespace cf {

class CLA_EXPO SharedResult
{
public:
    enum { MAXSIZE = 2 * 1024 * 1024 };
public:
    SharedResult();

    /**
      * Returns CEDPagePtr
      * If no data - returns empty CEDPagePtr()
      */
    CEDPagePtr get();

    /**
      * Stores CEDPage into result
      * Given serialized CEDPage have to be less then MAXSIZE
      * @return true on success, false on error
      */
    bool store(CEDPagePtr p);
private:
    void clear();
private:
    char data_[MAXSIZE];
    size_t size_;
};

}

#endif // SHAREDRESULT_H
