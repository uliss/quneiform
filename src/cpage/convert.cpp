/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include "convert.h"
#include "picture.h"
#include "polyblock.h"
#include "cpagetyps.h"
#include "cpage.h"
#include "cpage_debug.h"

namespace cf {
namespace cpage {

size_t pictureConvert(const Picture& pict, size_t sizeIn, PolyBlock * dataOut, size_t sizeOut)
{
    if (dataOut == NULL)
        return sizeof(PolyBlock);

    if (sizeof(PolyBlock) != sizeOut) {
        CPAGE_ERROR_FUNC << "invalid output data size:" << sizeOut;
        return 0;
    }

    if (sizeof(Picture) != sizeIn) {
        CPAGE_ERROR_FUNC << "invalid input data size:" << sizeIn;
        return 0;
    }

    dataOut->copyVertexes(pict);
    dataOut->setType(TYPE_PICTURE);
    dataOut->setNumber(0);
    return sizeof(PolyBlock);
}

}
}

