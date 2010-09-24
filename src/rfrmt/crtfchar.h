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

#ifndef CRTFCHAR_H_
#define CRTFCHAR_H_

#include <vector>
#include <boost/function.hpp>

#include "globus.h"
#include "cfcompat.h"
#include "common/lang_def.h"
#include "common/font.h"
#include "common/letter.h"
#include "common/rect.h"

namespace CIF
{

class SectorInfo;
class CEDChar;
class CEDLine;
class CEDParagraph;
class CRtfChar;

typedef boost::function<void(const CRtfChar*)> RfrmtDrawCharFunction;

class CLA_EXPO CRtfChar
{
    public:
        CRtfChar();
        CRtfChar(uchar chr, uchar probability = Letter::LOWEST_PROBABILITY);

        /**
         * Adds version
         * @throw std::out_of_range exception with number of versions exceeds RECT_MAX_VERS
         */
        void addVersion(const Letter& version);

        /**
         * Draws char bounding rects via previously set callbacks
         * @see setDrawCallback()
         */
        void drawLayout() const;

        /**
         * Returns true if char contains no versions
         */
        bool empty() const;

        /**
         * Returns first alternative
         */
        const Letter& first() const;

        /**
         * Returns first alternative
         */
        Letter& first();

        /**
         * Returns font number
         */
        font_number font() const;

        /**
         * Returns font size
         */
        short fontSize() const;

        /**
         * Returns first char version
         * @return 0 if no versions exists
         */
        unsigned char getChar() const;

        /**
         * Returns ideal char bounding rectangle
         */
        Rect idealRect() const;

        /**
         * Returns true if char is digit (0-9)
         */
        bool isDigit() const;

        /**
         * Returns true if char is dropped capital
         * \~russian буквица
         */
        bool isDropCap() const;

        /**
         * Returns if char spelled
         */
        bool isSpelled() const;

        /**
         * \~russin не знак переноса, а дефис в слове (пр: красно-белый)
         */
        bool isSpelledNoCarrying() const;

        /**
         * Returns character language
         */
        language_t language() const;

        /**
         * Returns bounding rectangle
         */
        Rect realRect() const;

        /**
         * Sets drop cap
         */
        void setDropCap(bool value);

        /**
         * Sets font number
         */
        void setFont(font_number fontNumber);

        /**
         * Sets font size
         */
        void setFontSize(short size);

        /**
         * Sets ideal rectangle
         * @param rect
         */
        void setIdealRect(const Rect& rect);

        /**
         * Sets character language
         * @param lang - new language
         */
        void setLanguage(language_t lang);

        /**
         * Sets real rectangle
         */
        void setRealRect(const Rect& rect);

        /**
         * Sets char spelled
         */
        void setSpelled(bool value);

        void setSpelledNoCarrying(bool value);

        /**
         * Returns pointer to CEDChar
         * @note caller should delete returned value
         */
        CEDChar * toCedChar(int font_name, int font_size, int font_style) const;

        /**
         * Inserts drop cap into CEDPage
         */
        CEDParagraph * insertCedDropCap(SectorInfo * sector, int font_name, int font_size,
                int font_style, bool negative) const;

        /**
         * Returns reference to character alternative
         * @param pos
         * @return const reference
         */
        const Letter& versionAt(size_t pos) const;

        /**
         * Returns reference to character alternative
         * @param pos
         * @return reference
         */
        Letter& versionAt(size_t pos);

        /**
         * Returns number of versions
         */
        size_t versionCount() const;

        /**
         * Write character to CEDLine
         * @return pointer to inserted CEDChar
         */
        CEDChar * write(CEDLine * line) const;
    public:
        static CEDChar * makeCedSpace(int fontName, int fontSize, int fontAttrs);
        static CRtfChar * read(FILE * in);
        static void setDrawCallback(RfrmtDrawCharFunction f);
    private:
        static RfrmtDrawCharFunction draw_func_;
    private:
        language_t language_;
        font_number font_number_;
        uint16_t font_size_;
        Rect ideal_rect_;
        Rect real_rect_;
        typedef std::vector<Letter> LetterList;
        LetterList versions_;
        bool spelled_;
        bool drop_cap_;
        bool spell_nocarrying_;
};

}

#endif /* CRTFCHAR_H_ */
