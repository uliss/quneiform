/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef CONVERT_H
#define CONVERT_H

#include "table.h"

void DefConvertInit();
uint32_t DefConvertBlock(uint32_t,
                          CDataType typeIn, const void *dataIn, uint32_t sizeIn,
                          CDataType typeOut, void * dataOut, uint32_t sizeOut);
uint32_t DefConvertPage(uint32_t context,
                         CDataType typeIn, const void * dataIn, uint32_t sizeIn,
                         CDataType typeOut, void * dataOut, uint32_t sizeOut);

uint32_t TYPE_DESK_to_CPAGE_TABLE(TABLE_DESC * lpDataIn, uint32_t SizeIn, CPAGE_TABLE * LpDataOut, uint32_t SizeOut);
uint32_t CPAGE_TABLE_to_TYPE_DESK(CPAGE_TABLE * lpDataIn, uint32_t SizeIn, TABLE_DESC * LpDataOut, uint32_t SizeOut);

namespace cf {
namespace cpage {

class PolyBlock;

size_t pictureToPolyBlock(const Picture * dataIn, PolyBlock * poly);
size_t pictureToPolyBlock(const void * dataIn, size_t sizeIn, void * dataOut, size_t sizeOut);
size_t polyBlockToPicture(const void * dataIn, size_t sizeIn, void * dataOut, size_t sizeOut);
}
}

#endif // CONVERT_H