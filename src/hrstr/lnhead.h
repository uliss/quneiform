/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef LNHEAD_H_
#define LNHEAD_H_

#include "cttypes.h"

enum lnhead_flag_t
{
    l_fbeg = 0x20,
    l_fend = 0x80,
    l_cbeg = 0x02,
    l_cend = 0x08
};

//-------------------- line representation ----------------------

//      At the beginning of line representation - word of total length -
//      not use it, simply skip
//      At end of each line zero byte as mark of line end
//      After last line zero word

//      line header
struct ln_head
{
        int16_t lth; // length of one line representation
        // ==head+intervals size in bytes
        int16_t h; // height of line == count of intervals
        int16_t row; // relative row of line start
        // (offset in pixels from top bound of comp, 0 based)
        uint16_t flg; // flags of free beg and free end
};
typedef struct ln_head lnhead;

#endif /* LNHEAD_H_ */
