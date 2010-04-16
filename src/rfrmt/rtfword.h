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

#ifndef RTFWORD_H_
#define RTFWORD_H_

#include <vector>
#include "cfcompat.h"

class CRtfChar;
class CRtfString;

// Word class
class CRtfWord
{
    public:
        CRtfWord();
        ~CRtfWord();

        CRtfChar* GetFirstChar();
        CRtfChar* GetNextChar();
        void get_coordinates_and_probability(void);

        std::vector<CRtfChar*> m_arChars;

        uint16_t m_wCharsCount;
        uint16_t m_wIndex;
        CRtfString* m_String;
        RECT m_rect;

        int16_t m_wcl;
        int16_t m_wct;
        int16_t m_wcr;
        int16_t m_wcb;
        int16_t m_wcs;
        int16_t m_wcp;

        uint16_t m_wFontNumber;
        uint16_t m_wIdealFontPointSize;
        uint16_t m_wRealFontPointSize;
};

#endif /* RTFWORD_H_ */
