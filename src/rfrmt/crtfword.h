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
#include <string>
#include "globus.h"
#include "cfcompat.h"
#include "common/rect.h"
#include "common/exception.h"

#include "font.h"

namespace CIF
{
class CRtfChar;
class CEDChar;
class CEDLine;

class CLA_EXPO CRtfWord
{
    public:
        CRtfWord();
        ~CRtfWord();

        typedef CIF::RuntimeExceptionImpl<CRtfWord> Exception;

        /**
         * Adds char to the end of the word
         */
        void addChar(CRtfChar * chr);

        /**
         * Adds char to the end of word
         */
        void addChar(uchar ch);

        /**
         * Returns bounding rectangle of word
         */
        Rect bRect() const;

        /**
         * Calculates word bounding rectangle, probability and spelling
         * @see bRect(), probability(), isSpelled()
         */
        void calcCoordinatesAndProbability();

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
         * Returns total length of word chars
         */
        int charTotalLength() const;

        /**
         * Removes all chars
         */
        void clearChars();

        /**
         * Returns true if word contains no chars
         */
        bool empty() const;

        /**
         * Returns true is word ends with given char
         */
        bool endsWith(int c) const;

        /**
         * Returns pointer to first char
         * @throw std::out_of_range if word is empty
         */
        CRtfChar * firstChar();
        const CRtfChar * firstChar() const;

        /**
         * Returns font attributes
         */
        int fontAttrs() const;

        /**
         * Returns word font number
         */
        font_number fontNumber() const;

        /**
         * Returns ideal font size for word
         */
        short idealFontSize() const;

        /**
         * Returns @b true if word is spelled
         */
        bool isSpelled() const;

        /**
         * Returns pointer to last char
         * @throw std::out_of_range if word is empty
         */
        CRtfChar * lastChar();
        const CRtfChar * lastChar() const;

        /**
         * Makes space
         */
        CEDChar * makeCedSpace(int fontName, int fontAttrs) const;

        /**
         * Returns word minimal probability
         */
        short probability() const;

        short realFontSize() const;

        /**
         * Sets word from string
         */
        void set(const std::string& str);

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
         * Returns true if word start from given char
         */
        bool startsWith(int c) const;

        /**
         * Returns word as string
         */
        std::string toString() const;

        /**
         * Writes word to CEDLine
         */
        void write(CEDLine * line) const;
    private:
        void calcBRect();
        void calcProbability();
        void calcSpelling();
        Rect charsBRect() const;
        bool charSpelling() const;
        short charProbability() const;

        /**
         * Rotates rectangle
         * @throw Exception - if wrong angle given
         */
        static void rotateRect(Rect& rect, int angle, int x_offset, int y_offset);
    private:
        typedef std::vector<CRtfChar*> CharList;
        typedef CharList::iterator CharIterator;
        typedef CharList::const_iterator CharIteratorConst;
        CharList chars_;
        font_number font_number_;
        short ideal_font_size_;
        short real_font_size_;
        bool spelling_;
        short probability_;
        Rect brect_;
};

}

#endif /* RTFWORD_H_ */
