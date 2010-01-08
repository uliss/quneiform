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

#ifndef BOX_H_
#define BOX_H_

#include "cfcompat.h"

struct MN;

//------------------- The box has a header ----------------------
struct BOX
{
        BOX * boxnext; // chain address (zero if no next box)
        MN * boxmain; // component main number pointer
        uint16_t boxptr; // ptr to the empty place in the box
        int16_t boxleft; // left boundary for line envelope
        int16_t boxright; // right boundary for line envelope
        int16_t boxey; // row of line end+1 ( if line ends within
        //    box)
        int16_t boxel; // length of the last segment (if line ends
        //    within box)
        int16_t boxex; // coordinate of last segment end (if line
        //    ends within box)
        uchar boxflag; // byte for box attributes flags
        uchar boxwf; // working flag (for picture compress)
        uint16_t boxresw; // reserved word (for *4 arround)
};

// Values of boxflag:
enum box_flag_t
{
    BOXMARK = 1, // flag for temporary box marking
    BOXPICTURE = 2, // the picture box - only header in the box is true
    BOXFREEBEG = 0x30, // line start type mask
    BOXFREEEND = 0xc0, // line end type mask
    //      The 'boxfreebeg' and 'boxfreeend' bites has following meaning:
    //      '11' - free begin/end of line
    //      '01' - not free begin/end
    //      '00' - no information about begin/end in this box
    BOXBEG = 0x10,// line start not free
    BOXEND = 0x40,// line end not free
    // BOX size
    BOXHLTH = (sizeof(BOX)), // the box header length
    BOXSIZE = (BOXHLTH + 32 * 4), // the length of box
    BOXBOUNDARY = (BOXSIZE - 6)
// the boundary control
};

struct box_interval
{
        int16_t l; // length of interval
        int16_t d; // displacement of the end of the current interval relative
}; //      to the previous
typedef struct box_interval BOXINT;

#endif /* BOX_H_ */
