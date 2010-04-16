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

#ifndef CRTFSTRING_H_
#define CRTFSTRING_H_

#include <vector>
#include "cfcompat.h"

class CRtfFragment;
class CRtfWord;

class CRtfString
{
    public:
        CRtfString();
        ~CRtfString();

        CRtfWord* GetFirstWord(void);
        CRtfWord* GetNextWord(void);
        uint16_t get_max_font_size(void);
        int16_t GetStringSizeInTwips(void);
        uint16_t GetRealStringSize(void);

        std::vector<CRtfWord*> m_arWords;

        uint16_t m_wWordsCount;
        uint16_t m_wIndex;
        CRtfFragment* m_Frag;
        RECT m_rect;
        uint16_t m_wType;

        int32_t m_LeftBorder;
        int32_t m_RightBorder;
        uint16_t m_wLeftIndent;
        uint16_t m_wRightIndent;
        uint16_t m_wCentre;
        uint16_t m_wFirstIndent;
        uint16_t m_wFlagBeginParagraph;
        uint16_t m_wAlignment;
        uint16_t m_wPrevAlignment;
        uint16_t m_LengthStringInTwips;
        uint16_t m_wSpaceBefore;
        uchar m_wLeftBorderEqual;
        uchar m_wRightBorderEqual;
        uchar m_wCentreEqual;
        uchar m_bLineTransfer;
        uchar m_LastChar;
        uchar m_FirstChar;
        uchar m_Attr;
        uchar m_FlagCarry;
        uint32_t S_Flags; //NEGA_STR vmk 10-06-2001
};

#endif /* CRTFSTRING_H_ */
