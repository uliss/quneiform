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

#ifndef BMP_H
#define BMP_H

#include <iosfwd>

#include "common/dib.h"
#include "cttypes.h"
#include "globus.h"

namespace cf {

PACKED_STRUCT_PROLOGUE
struct BitmapFileHeader
{
public:
    BitmapFileHeader() :
        bfType(0x4d42),
        bfSize(0),
        bfReserved1(0),
        bfReserved2(0),
        bfOffBits(0) {}

    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} PACKED_STRUCT_EPILOGUE;

}

#endif // BMP_H
