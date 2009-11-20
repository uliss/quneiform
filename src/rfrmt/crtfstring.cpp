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

#include "crtfstring.h"
#include "crtfword.h"
#include "creatertf.h"
#include "size.h"
#include "frmtpict.h"

#include <cstring>

extern uint32_t FlagMode;
int16_t get_font_name(int16_t FontNumber);
using namespace CIF;

int16_t GetRealSize(int16_t FontSize, int16_t FontNumber, int16_t* strHeight) {
    Size size;
    int n_Weight = 600, fn;
    uchar bItalic;

    if (!(FlagMode & NOBOLD) && ((char) FontNumber & TG_EDW_BOLD))
        n_Weight = 800;

    if (FlagMode & NOSIZE)
        FontSize = DefFontSize / 2;

    if (!(FlagMode & NOCURSIV) && ((char) FontNumber & TG_EDW_ITALIC))
        bItalic = TRUE;
    else
        bItalic = FALSE;

    fn = get_font_name(FontNumber);

    //  TODO
    *strHeight = size.height();
    return size.width(); //in twips
}

CRtfString::CRtfString() {
    left_indent = 0;
    right_indent = 0;
    first_indent = 0;
    flag_begin_paragraph = 0;
    alignment = 0;
    prev_alignment = 0;
    left_border_equal = 0;
    right_border_equal = 0;
    centre_equal = 0;
    line_transfer = 0;
    flag_carry = 0;
    flags = 0;
    length_string_in_twips = 0;
}

CRtfString::~CRtfString() {
    for(vectorWord::iterator it = words.begin(), end = words.end(); it != end; ++it)
        delete *it;
}

uint16_t CRtfString::GetRealStringSize(void) {
    char tmp_str[MAX_BUFFER_SIZE];

    int index = 0;
    for (uint nw = 0; nw < words.size(); nw++) {
        CRtfWord* word = words[nw];
        for (uint nz = 0; nz < word->chars.size(); nz++) {
            tmp_str[index++] = word->chars[nz]->versions[0].char_;
        }
        tmp_str[index++] = ' ';
    }

    tmp_str[index] = 0;
    CRtfWord* pRtfWord = words[0];
    CRtfChar *pRtfChar = pRtfWord->chars[0];
    int16_t strHeight;
    return GetRealSize(pRtfWord->real_font_point_size, pRtfChar->fontNumber, &strHeight);
}
