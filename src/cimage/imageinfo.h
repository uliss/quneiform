/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef IMAGEINFO_H_
#define IMAGEINFO_H_

#include "cttypes.h"

struct CIMAGE_ImageInfo {
    CIMAGE_ImageInfo() : wImageHeight(0), wImageWidth(0), wImageByteWidth(0),
        wImageDisplacement(0), wResolutionX(0), wResolutionY(0), bFotoMetrics(0),
        bUnused(0), wAddX(0), wAddY(0) {}
public:
    uint16_t wImageHeight;
    uint16_t wImageWidth;
    uint16_t wImageByteWidth;
    uint16_t wImageDisplacement;
    uint16_t wResolutionX;
    uint16_t wResolutionY;
    uchar bFotoMetrics;
    uchar bUnused;
    uint16_t wAddX;
    uint16_t wAddY;
};

typedef Bool16 (*PCIMAGE_Callback_ImageOpen)(CIMAGE_ImageInfo*);
typedef uint16_t (*PCIMAGE_Callback_ImageRead)(pchar, uint16_t);
typedef Bool16 (*PCIMAGE_Callback_ImageClose)(void);

struct CIMAGEIMAGECALLBACK {
    PCIMAGE_Callback_ImageOpen CIMAGE_ImageOpen;
    PCIMAGE_Callback_ImageRead CIMAGE_ImageRead;
    PCIMAGE_Callback_ImageClose CIMAGE_ImageClose;
};

struct CIMAGE_InfoDataInGet {
    CIMAGE_InfoDataInGet() :
        dwX(0), dwY(0), dwWidth(0), dwHeight(0), wByteWidth(0) {}
public:
    uint32_t dwX;
    uint32_t dwY;
    uint32_t dwWidth;
    uint32_t dwHeight;
    uint16_t wByteWidth;
};

struct CIMAGE_InfoDataOutGet {
    CIMAGE_InfoDataOutGet() :
        dwWidth(0), dwHeight(0), wByteWidth(0), byBit(0), wBlackBit(0), lpData(0) {}
public:
    uint32_t dwWidth;
    uint32_t dwHeight;
    uint16_t wByteWidth;
    uint16_t byBit;
    uint32_t wBlackBit;
    puchar lpData;
};

struct CIMAGE_InfoDataInReplace {
    CIMAGE_InfoDataInReplace() :
        byBit(0), dwX(0), dwY(0), dwWidth(0), dwHeight(0), wByteWidth(0), lpData(0),
        Reserved(0), wReserved(0), MaskFlag(0) {}
    uint16_t byBit;
    uint32_t dwX;
    uint32_t dwY;
    uint32_t dwWidth;
    uint32_t dwHeight;
    uint16_t wByteWidth;
    puchar lpData;
    uchar Reserved;
    uint16_t wReserved;
    uchar MaskFlag;
};

struct BitmapInfoHeader {
public:
    BitmapInfoHeader() :
            biSize(0), biWidth(0), biHeight(0), biPlanes(0), biBitCount(0),
            biCompression(0), biSizeImage(0), biXPelsPerMeter(0),
            biYPelsPerMeter(0), biClrUsed(0), biClrImportant(0) {
    }

    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

typedef BitmapInfoHeader * BitmapHandle;

#endif
