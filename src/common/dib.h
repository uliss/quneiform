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

#ifndef DIB_H
#define DIB_H

#include <boost/static_assert.hpp>

#include "cttypes.h"

namespace cf {

// DIB version 3 header (length - 40 bytes)
PACKED_STRUCT_PROLOGUE
struct BitmapInfoHeader
{
    uint32_t        biSize;
    int32_t         biWidth;
    int32_t         biHeight;
    uint16_t        biPlanes;
    uint16_t        biBitCount;
    uint32_t        biCompression;
    uint32_t        biSizeImage;
    int32_t         biXPelsPerMeter;
    int32_t         biYPelsPerMeter;
    uint32_t        biClrUsed;
    uint32_t        biClrImportant;
public:
    bool operator==(const BitmapInfoHeader& r) const
    {
        return biSize == r.biSize &&
                biWidth == r.biWidth &&
                biHeight == r.biHeight &&
                biPlanes == r.biPlanes &&
                biBitCount == r.biBitCount &&
                biCompression == r.biCompression &&
                biSizeImage == r.biSizeImage &&
                biXPelsPerMeter == r.biXPelsPerMeter &&
                biYPelsPerMeter == r.biYPelsPerMeter &&
                biClrUsed == r.biClrUsed &&
                biClrImportant == r.biClrImportant;
    }
} PACKED_STRUCT_EPILOGUE;

struct DibXYZ {
    int32_t ciexyzX;
    int32_t ciexyzY;
    int32_t ciexyzZ;
};

struct DibXYZTriple {
    DibXYZ  ciexyzRed;
    DibXYZ  ciexyzGreen;
    DibXYZ  ciexyzBlue;
};

// DIB version 4 header (length - 108 bytes)
PACKED_STRUCT_PROLOGUE
struct BitmapInfoV4Header {
    uint32_t             bV4Size;
    int32_t              bV4Width;
    int32_t              bV4Height;
    uint16_t             bV4Planes;
    uint16_t             bV4BitCount;
    uint32_t             bV4V4Compression;
    uint32_t             bV4SizeImage;
    int32_t              bV4XPelsPerMeter;
    int32_t              bV4YPelsPerMeter;
    uint32_t             bV4ClrUsed;
    uint32_t             bV4ClrImportant;
    uint32_t             bV4RedMask;
    uint32_t             bV4GreenMask;
    uint32_t             bV4BlueMask;
    uint32_t             bV4AlphaMask;
    uint32_t             bV4CSType;
    DibXYZTriple         bV4Endpoints;
    uint32_t             bV4GammaRed;
    uint32_t             bV4GammaGreen;
    uint32_t             bV4GammaBlue;
} PACKED_STRUCT_EPILOGUE;

// DIB version 5 header (length - 124 bytes)
PACKED_STRUCT_PROLOGUE
struct BitmapInfoV5Header {
    uint32_t             bV5Size;
    int32_t              bV5Width;
    int32_t              bV5Height;
    uint16_t             bV5Planes;
    uint16_t             bV5BitCount;
    uint32_t             bV5Compression;
    uint32_t             bV5SizeImage;
    int32_t              bV5XPelsPerMeter;
    int32_t              bV5YPelsPerMeter;
    uint32_t             bV5ClrUsed;
    uint32_t             bV5ClrImportant;
    uint32_t             bV5RedMask;
    uint32_t             bV5GreenMask;
    uint32_t             bV5BlueMask;
    uint32_t             bV5AlphaMask;
    uint32_t             bV5CSType;
    DibXYZTriple         bV5Endpoints;
    uint32_t             bV5GammaRed;
    uint32_t             bV5GammaGreen;
    uint32_t             bV5GammaBlue;
    uint32_t             bV5Intent;
    uint32_t             bV5ProfileData;
    uint32_t             bV5ProfileSize;
    uint32_t             bV5Reserved;
} PACKED_STRUCT_EPILOGUE;

enum {
    CTDIB_BI_JPEG = 4L
};

PACKED_STRUCT_PROLOGUE
struct RGBQuad { // rgbq
    RGBQuad() :
        rgbBlue(0),
        rgbGreen(0),
        rgbRed(0),
        rgbReserved(0) {}

    RGBQuad(uchar r, uchar g, uchar b, uchar a = 0) :
        rgbBlue(b),
        rgbGreen(g),
        rgbRed(r),
        rgbReserved(a) {}

    bool operator==(const RGBQuad& r) const {
        return rgbBlue == r.rgbBlue &&
                rgbGreen == r.rgbGreen &&
                rgbRed == r.rgbRed;
    }
public:
    static RGBQuad black() { return RGBQuad(0, 0, 0, 0); }
    static RGBQuad white() { return RGBQuad(255, 255, 255, 0); }
    static RGBQuad red() { return RGBQuad(255, 0, 0); }
    static RGBQuad green() { return RGBQuad(0, 255, 0); }
    static RGBQuad blue() { return RGBQuad(0, 0, 255); }
public:
    uchar    rgbBlue;
    uchar    rgbGreen;
    uchar    rgbRed;
    uchar    rgbReserved;
} PACKED_STRUCT_EPILOGUE;

enum {
    DIB_VERSION_3_HEADER_SIZE = sizeof(BitmapInfoHeader),     //40
    DIB_VERSION_4_HEADER_SIZE = sizeof(BitmapInfoV4Header),   //108
    DIB_VERSION_5_HEADER_SIZE = sizeof(BitmapInfoV5Header)    //124
};

BOOST_STATIC_ASSERT(sizeof(BitmapInfoHeader) == 40);
BOOST_STATIC_ASSERT(sizeof(BitmapInfoV4Header) == 108);
BOOST_STATIC_ASSERT(sizeof(BitmapInfoV5Header) == 124);

typedef cf::BitmapInfoHeader * BitmapPtr;

}

#endif // DIB_H
