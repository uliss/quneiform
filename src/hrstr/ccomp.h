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

#ifndef CCOMP_H_
#define CCOMP_H_

#include "cttypes.h"

//-------------------- component in kit --------------------------

struct c_comp
{
    uint16_t size; // size of component in paragraphs >=3
    int16_t upper; // upper boundary of component
    int16_t left; // left boundary of component
    int16_t h; // height of component
    int16_t w; // width of component
    uchar rw; // raster width in bytes
    uchar type; // recognition type

    enum
    {
        ch_perfect = 1, // perfect type defined
        ch_letter = 2, // letter type
        ch_dust = 4, // dust type
        ch_punct = 8, // punctuation
        ch_stick = 16, // vertical bar component
        ch_great = 32, // great component
        ch_merge = 64, // merged components
        ch_notltr = 128
    // not letter or punctuation
    };

    uchar cs; // recognition case (see bellow)
    uchar pidx; // proportional index (ref.)
    int16_t nvers; // number of alternative versions
    int16_t records; // recognition records offset
    int16_t lines; // ptr to line representation
    int16_t nl; // number of lines
    uchar begs; // number of free begins
    uchar ends; // number of free ends
    uchar reasno; // proportional criteria messages
    uchar large; // large type
#define ch_underlined   1   // component was underlined
#define ch_taken    2   // taken to line at dust_ini
    uchar scale; // scale of the component
    c_comp * next;
};

#endif /* CCOMP_H_ */
