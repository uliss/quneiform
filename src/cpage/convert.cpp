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

size_t pictureToPolyBlock(const void * dataIn, size_t sizeIn, void * dataOut, size_t sizeOut)
{
    if (dataOut == NULL)
        return sizeof(PolyBlock);

    if(dataIn == NULL)
        return 0;

    if (sizeof(PolyBlock) != sizeOut) {
        CPAGE_ERROR_FUNC << "invalid output data size:" << sizeOut;
        return 0;
    }

    if (sizeof(Picture) != sizeIn) {
        CPAGE_ERROR_FUNC << "invalid input data size:" << sizeIn;
        return 0;
    }

    const Picture * pict = static_cast<const Picture*>(dataIn);
    PolyBlock * poly = static_cast<PolyBlock*>(dataOut);
    return pictureToPolyBlock(pict, poly);
}

size_t pictureToPolyBlock(const Picture * pict, PolyBlock * poly)
{
    if(!pict || !poly)
        return 0;

    poly->copyVertexes(*pict);
    poly->setType(TYPE_PICTURE);
    poly->setNumber(0);
    return sizeof(PolyBlock);
}

size_t polyBlockToPicture(const void * dataIn, size_t sizeIn, void * dataOut, size_t sizeOut)
{
    if(sizeof(cpage::PolyBlock) != sizeIn) {
        CPAGE_ERROR_FUNC << "invalid input data size:" << sizeIn;
        return 0;
    }

    if(!dataOut)
        return sizeof(cpage::Picture);

    if(!dataIn)
        return 0;

    if(sizeOut != sizeof(cpage::Picture)) {
        CPAGE_ERROR_FUNC << "invalid output data size:" << sizeOut;
        return 0;
    }

    const cpage::PolyBlock * poly = static_cast<const cpage::PolyBlock*>(dataIn);
    cpage::Picture * pict = static_cast<cpage::Picture*>(dataOut);
    pict->set(*poly);
    return sizeof(cpage::Picture);
}

}
}

