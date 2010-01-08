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

#ifndef RSTR_STRING_H_
#define RSTR_STRING_H_

#include "cfcompat.h"
#include "comp.h"

//------------------- string of letters ------------------------

struct str
{
        int16_t row; // upper of line
        int16_t col; // left of line
        int16_t lower; // lower of line
        int16_t right; // right of line
        uint16_t dust; // end of letter ptrs
        uint16_t end; // end of dust ptrs
        uint16_t lth;
        uint16_t first;
        uint16_t last;
        uint16_t scale; // scale of the string
        int16_t fragment; // fragment of the string
        uchar language; // language of the string
        c_comp *ccomp[1]; // array of ptrs to components
};

#endif /* RSTR_STRING_H_ */
