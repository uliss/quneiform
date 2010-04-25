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
#include <string>
#include "cfcompat.h"
#include "common/rect.h"

namespace CIF
{

class CRtfChar;
class CRtfWord;

class CLA_EXPO CRtfString
{
    public:
        CRtfString();
        ~CRtfString();

        /**
         * Adds word to the end of string
         */
        void addWord(CRtfWord * word);

        /**
         * Returns string center coord
         */
        int center() const;

        /**
         * Removes all words from string
         */
        void clearWords();

        /**
         * Returns true if string is empty
         */
        bool empty() const;

        /**
         * Returns true if string ends with given char
         */
        bool endsWith(int c) const;

        /**
         * Returns first char in string
         * @return NULL if string is empty
         */
        CRtfChar * firstChar();
        const CRtfChar * firstChar() const;

        /**
         * Returns pointer to first word
         * @throw std::out_of_range if string is empty
         */
        CRtfWord * firstWord();
        const CRtfWord * firstWord() const;

        /**
         * Checks is string has attributes
         */
        bool hasAttributes() const;

        bool isEqualCenter() const;
        bool isEqualLeft() const;
        bool isEqualRight() const;

        /**
         * Returns true if string begin paragraph
         */
        bool isParagraphBegin() const;

        /**
         * Returns last char in string
         * @return 0 if string is empty
         */
        CRtfChar * lastChar();
        const CRtfChar * lastChar() const;

        /**
         * Returns pointer to last word in string
         * @throw std::out_of_range if string is empty
         */
        CRtfWord * lastWord();
        const CRtfWord * lastWord() const;

        /**
         * Returns left string border
         * @throw std::out_of_range if string is empty
         */
        int leftBorder() const;

        /**
         * Returns left indent
         */
        int leftIndent() const;

        /**
         * Returns string length in twips
         */
        int lengthInTwips() const;

        /**
         * Returns true if string in carrying
         */
        bool lineCarry() const;

        /**
         * Returns string line transfer
         */
        bool lineTransfer() const;

        /**
         * Returns max font size among string words
         */
        int maxWordFontSize() const;

        /**
         * Returns real string length in twips
         */
        uint realLength() const;

        /**
         * Returns right border of string
         * @throw std::out_of_range if string is empty
         */
        int rightBorder() const;

        /** */
        void setAttributes(bool value);

        void setEqualCenter(bool value);
        void setEqualLeft(bool value);
        void setEqualRight(bool value);

        /**
         * Sets string left indent
         */
        void setLeftIndent(int indent);

        /**
         * Sets line carrying
         */
        void setLineCarry(bool value);

        /**
         * Sets line transfer
         */
        void setLineTransfer(bool value);

        /**
         * Sets string paragraph begining
         */
        void setParagraphBegin(bool value);

        /**
         * Checks if string starts with symbol chr
         */
        bool startsWith(int c) const;

        /**
         * Checks if string starts with dash
         */
        bool startsWithDash() const;

        /**
         * Checs if string starts with digit
         */
        bool startsWithDigit() const;

        /**
         * Returns string content as std::string
         */
        std::string toString() const;

        /**
         * Returns distance between left and right border
         * @return 0 if string is empty
         */
        int width() const;

        /**
         * Returns pointer to word at position @b pos
         * @throw std::out_of_range if no such position
         */
        CRtfWord * wordAt(size_t pos);
        const CRtfWord * wordAt(size_t pos) const;

        /**
         * Returns words count
         */
        size_t wordCount() const;

        uint16_t m_wRightIndent;
        uint16_t m_wFirstIndent;
        uint16_t m_wAlignment;
        uint16_t m_LengthStringInTwips;
        uint16_t m_wSpaceBefore;
        uint32_t S_Flags; //NEGA_STR vmk 10-06-2001
    private:
        typedef std::vector<CRtfWord*> WordList;
        typedef WordList::iterator WordIterator;
        typedef WordList::const_iterator WordIteratorConst;
        WordList words_;
        bool line_break_;
        bool paragraph_begin_;
        bool carry_;
        bool has_attributes_;
        bool equal_center_;
        bool m_wLeftBorderEqual;
        bool m_wRightBorderEqual;
        int left_indent_;
};

}

#endif /* CRTFSTRING_H_ */
