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

#ifndef CRTFSTRING_H_
#define CRTFSTRING_H_

#include <vector>
#include <iostream>

#include "cttypes.h"

class CRtfWord;
class CRtfFragment;

class CRtfString
{
public:
    CRtfString();
    ~CRtfString();

    uint16_t GetRealStringSize(void);

    typedef std::vector<CRtfWord*> vectorWord;
    vectorWord words;

    int32_t left_border;
    int32_t right_border;
    uint16_t left_indent;
    uint16_t right_indent;
    uint16_t centre;
    uint16_t first_indent;
    uint16_t flag_begin_paragraph;
    uint16_t alignment;
    uint16_t prev_alignment;
    uint16_t length_string_in_twips;
    uint16_t space_before;
    uchar left_border_equal;
    uchar right_border_equal;
    uchar centre_equal;
    uchar line_transfer;
    uchar last_char;
    uchar first_char;
    uchar attr;
    uchar flag_carry;
    uint32_t flags; //NEGA_STR vmk 10-06-2001
};

std::ostream& operator<<(std::ostream& os, const CRtfString& string);

#endif /* CRTFSTRING_H_ */
