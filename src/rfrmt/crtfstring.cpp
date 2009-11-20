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
    words_count = 0;
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
    CRtfWord* cWord;

    words_count = words.size();
    for (int i = 0; i < words_count; i++) {
        cWord = words[i];
        delete cWord;
    }
}

int16_t CRtfString::GetStringSizeInTwips() {
    CRtfWord* pRtfWord;
    CRtfChar *pLastChar, *pFirstChar;
    uint16_t CountChars;

    pRtfWord = (CRtfWord*) words[0];
    pFirstChar = (CRtfChar*) pRtfWord->chars[0];
    pRtfWord = (CRtfWord*) words[words_count - 1];
    CountChars = pRtfWord->chars.size();
    pLastChar = (CRtfChar*) pRtfWord->chars[CountChars - 1];
    int16_t LenghtStr =
            (int16_t) ((pLastChar->ideal_rect_.right() - pFirstChar->ideal_rect_.left()) * Twips);
    return LenghtStr;
}

uint16_t CRtfString::GetRealStringSize(void) {
    char tmp_str[MAX_BUFFER_SIZE];
    CRtfWord* pRtfWord;
    CRtfChar *pRtfChar;

    uint16_t CountChars, RealSize;
    int16_t strHeight;
    int index = 0;

    for (int nw = 0; nw < words_count; nw++) {
        pRtfWord = (CRtfWord*) words[nw];
        CountChars = pRtfWord->chars.size();
        for (int nz = 0; nz < CountChars; nz++) {
            pRtfChar = (CRtfChar*) pRtfWord->chars[nz];
            tmp_str[index++] = pRtfChar->versions[0].char_;

        }
        tmp_str[index++] = ' ';
    }

    tmp_str[index] = 0;
    pRtfWord = (CRtfWord*) words[0];
    pRtfChar = (CRtfChar*) pRtfWord->chars[0];

    RealSize = GetRealSize(pRtfWord->real_font_point_size, pRtfChar->fontNumber, &strHeight);
    return RealSize;
}

CRtfWord* CRtfString::GetNextWord() {
    words.push_back(new CRtfWord());
    return words.back();
}

uint16_t CRtfString::get_max_font_size() {
    uint16_t nw, str_max_font = 3;
    CRtfWord* pRtfWord;

    for (nw = 0; nw < words_count; nw++) {
        pRtfWord = (CRtfWord*) words[nw];
        str_max_font = MAX(str_max_font, pRtfWord->real_font_point_size);
    }
    return str_max_font;
}
