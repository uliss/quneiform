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

#ifndef ROOTS_STRUCT_H
#define ROOTS_STRUCT_H

#include "cttypes.h"

enum {
    ROOT_NULL           = 0x00,
    ROOT_LETTER         = 0x01,
    ROOT_USED           = 0x02,
    ROOT_RECOGNIZED     = 0x04,
    ROOT_DUST           = 0x08,
    ROOT_SPECIAL_LETTER = 0x10,
    ROOT_SPECIAL_DUST   = 0x20,
    ROOT_HORZ_BREAKED   = 0x40
};

enum root_block_number_t {
    REMOVED_BLOCK_NUMBER       = -1,
    DUST_BLOCK_NUMBER          = 0,
    REMAINDER_BLOCK_NUMBER     = 1,
    FIRST_REGULAR_BLOCK_NUMBER = 2
};

struct CCOM_comp;

enum {
    MAX_DUST_WIDTH  = 8,
    MAX_DUST_HEIGHT = 8
};

struct ROOT
{
    int16_t yRow;
    int16_t xColumn;

    union {
        struct {
            uint16_t wSegmentPtr;
            uint16_t wLength;
        } u2;

        ROOT *pNext;
    } u1;

    int16_t nHeight;
    int16_t nWidth;
    uchar bType;
    uchar bReached;
    int16_t nBlock;  // тип блока: "дустовый" али какой
    int16_t nUserNum;
    CCOM_comp * pComp;
public:
    bool isLayoutDust() const {
        return nWidth <= MAX_DUST_WIDTH && nHeight <= MAX_DUST_HEIGHT;
    }
};

struct ROOT_EXT
{
    uint16_t wSegmentPtr;
    uint16_t wLength;
};

#endif // ROOTS_STRUCT_H
