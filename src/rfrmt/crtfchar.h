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

#ifndef CRTFCHAR_H_
#define CRTFCHAR_H_

#include "cttypes.h"
#include "rect.h"
#include "recdefs.h"
#include <boost/array.hpp>

class CRtfChar
{
public:
    struct Versions
    {
        uchar char_;
        uchar probability_;
    };

    boost::array<Versions, REC_MAX_VERS> versions;

    CIF::Rect ideal_rect_;
    CIF::Rect real_rect_;
    uchar language_;
    uchar flag_spell;
    uchar flag_spell_nocarrying;
    uchar flag_cup_drop;
    uint16_t countAlt;
    uint16_t fontNumber;
    uint16_t fontPointSize;
};

#endif /* CRTFCHAR_H_ */
