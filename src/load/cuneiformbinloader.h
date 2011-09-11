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

#ifndef CUNEIFORMBINLOADER_H
#define CUNEIFORMBINLOADER_H

#include <iosfwd>

#include "globus.h"
#include "ced/cedpageptr.h"
#include "common/exception.h"

namespace cf {

class CLA_EXPO CuneiformBinLoader
{
public:
     typedef RuntimeExceptionImpl<CuneiformBinLoader> Exception;
public:
    CuneiformBinLoader();

    /**
      * Loads CEDPage from given binary stream
      * @throw Exception on load error
      * @return CEDPage shared pointer
      * @param is - input stream
      */
    CEDPagePtr load(std::istream& is);
};

}

#endif // CUNEIFORMBINLOADER_H
