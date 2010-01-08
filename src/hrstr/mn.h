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

#ifndef MN_H_
#define MN_H_

#include "cttypes.h"

struct BOX;

const int usual_box_count = 20; // heuristic of number of lines in a letter
const int great_box_count = 200; // heuristic for number of boxes in a picture

enum
{
    // component is a picture
    mnpicture = 1
};

struct MN
{
        BOX *mnfirstbox; // address of the first box
        int16_t mncounter; // number of living lines in the component
#define mnfree  mnfirstbox  // reference to next free main number
        int16_t mnupper; // upper bound of component
        int16_t mnlower; // lower bound of component
        int16_t mnboxcnt; // number of boxes in component
        uchar mnlines; // number of lines in the component
        uchar mnbegs; // number of free line begins
        uchar mnends; // number of free line ends
        uchar mnflag; // flag byte for main number
        MN *mnnext; // address of next dead component
};

#endif /* MN_H_ */
