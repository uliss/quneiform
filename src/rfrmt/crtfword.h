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
#include "common/rect.h"

#include "crtfstruct.h"

namespace CIF
{
class CRtfChar;
class CRtfString;

class CRtfWord
{
    public:
        CRtfWord();
        ~CRtfWord();

        /**
         * Adds char to the end of the word
         */
        void addChar(CRtfChar * chr);

        /**
         * Returns bounding rectangle of word
         */
        Rect bRect() const;

        /**
         * Returns pointer to character at position @pos
         * @throw std::out_of_range exception if invalid position given
         */
        CRtfChar * charAt(size_t pos);

        /**
         * Returns number of chars is word
         */
        size_t charCount() const;

        /**
         * Removes all chars
         */
        void clearChars();

        /**
         *
         */
        void getCoordinatesAndProbability();

        /**
         * Returns pointer to first char
         * @throw std::out_of_range if word is empty
         */
        CRtfChar * firstChar();
        const CRtfChar * firstChar() const;

        /**
         * Returns word font number
         */
        font_number fontNumber() const;

        /**
         * Returns ideal font size for word
         */
        short idealFontSize() const;

        /**
         * Returns pointer to last char
         * @throw std::out_of_range if word is empty
         */
        CRtfChar * lastChar();
        const CRtfChar * lastChar() const;

        /**
         * Returns word minimal probability
         */
        short probability() const;

        short realFontSize() const;

        /**
         * Sets word font number
         */
        void setFontNumber(font_number number);

        /**
         * Sets ideal font size
         */
        void setIdealFontSize(short size);

        /**
         * Sets real font size
         */
        void setRealFontSize(short size);

        /**
         * Returns word spelling
         */
        bool spelling() const;
    private:
        void calcMinProbability();
        void calcMinSpelling();
        Rect charsBRect() const;
        void rotateRect(Rect& rect, int angle, int x_offset, int y_offset);
    private:
        typedef std::vector<CRtfChar*> CharList;
        CharList chars_;
        font_number font_number_;
        short ideal_font_size_;
        short real_font_size_;
        short spelling_;
        short probability_;
        Rect brect_;
        int16_t m_wcl;
        int16_t m_wct;
        int16_t m_wcr;
        int16_t m_wcb;

};

}

#endif /* RTFWORD_H_ */
