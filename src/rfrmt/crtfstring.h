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
#include <iostream>
#include "font.h"
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
         * @see clearWords(), wordAt()
         */
        void addWord(CRtfWord * word);

        /**
         * Returns string aling
         * @see setAlign()
         */
        format_align_t align() const;

        /**
         * Calculates string width
         */
        void calcRealLength();

        /**
         * Returns string center coord
         * @see width(), leftBorder(), rightBorder()
         */
        int center() const;

        /**
         * Returns number of chars in string
         */
        size_t charCount() const;

        /**
         * Returns total length of chars
         */
        int charTotalLength() const;

        /**
         * Removes all words from string
         * @see addWord()
         */
        void clearWords();

        /**
         * Returns true if string contains no words
         * @see wordCount()
         */
        bool empty() const;

        /**
         * Returns true if string ends with given char
         * @see startsWith()
         */
        bool endsWith(int c) const;

        /**
         * Returns first char in string
         * @throw std::out_of_range if string or first word is empty
         * @see lastChar(), firstWord()
         */
        CRtfChar * firstChar();
        const CRtfChar * firstChar() const;

        /** */
        int firstIndent() const;

        /**
         * Returns pointer to first word
         * @throw std::out_of_range if string is empty
         * @see lastWord(), firstChar()
         */
        CRtfWord * firstWord();
        const CRtfWord * firstWord() const;

        /**
         * Returns CSTR flags
         * @see hasFlag(), setFlags()
         */
        uint flags() const;

        /**
         * Checks is string has attributes
         * @see attributes()
         */
        bool hasAttributes() const;

        /**
         * Returns true if string has at least one big space between words
         * @max_space maximum space between words
         */
        bool hasBigSpace(int max_space) const;

        /**
         * Returns true if string contains chars
         */
        bool hasChars() const;

        /**
         * Checks if string has CSTR flag
         * @see flags(), setFlags()
         */
        bool hasFlag(uint flag) const;

        bool isEqualCenter() const;
        bool isEqualLeft() const;
        bool isEqualRight() const;

        /**
         * Returns true if line carry needed
         */
        bool isLineCarryNeeded() const;

        /**
         * Returns true if string begin paragraph
         */
        bool isParagraphBegin() const;

        /**
         * Returns last char in string
         * @throw std::out_of_range if string or last word is empty
         * @see lastWord(), firstChar()
         */
        CRtfChar * lastChar();
        const CRtfChar * lastChar() const;

        /**
         * Returns pointer to last word in string
         * @throw std::out_of_range if string is empty
         * @see lastChar(), wordAt(), firstWord()
         */
        CRtfWord * lastWord();
        const CRtfWord * lastWord() const;

        /**
         * Returns left string border
         * @throw std::out_of_range if string is empty
         * @see rightBorder()
         */
        int leftBorder() const;

        /**
         * Returns left indent
         * @see rightIndent(), setLeftIndent()
         */
        int leftIndent() const;

        /**
         * Returns string length in twips
         */
        int lengthInTwips() const;

        /**
         * Returns true if string in carrying
         * @see setLineCarry()
         */
        bool lineCarry() const;

        /**
         * Returns string line transfer
         * @see setLineTransfer()
         */
        bool lineTransfer() const;

        /**
         * Returns string top margin
         * @see setMarginTop()
         */
        int marginTop() const;

        /**
         * Returns max font size among string words
         */
        int maxWordFontSize() const;

        /**
         * Prints string data to given stream
         */
        void print(std::ostream& os) const;

        /**
         * Returns real string length in twips
         * @see width()
         */
        uint realLength() const;

        /**
         * Returns right border of string
         * @throw std::out_of_range if string is empty
         * @see leftBorder()
         */
        int rightBorder() const;

        /**
         * Returns string right indent
         * @see setRightIndent(), leftIndent()
         */
        int rightIndent() const;

        /**
         * Sets string alignment
         * @see align()
         */
        void setAlign(format_align_t align);

        /** */
        void setAttributes(bool value);

        void setEqualCenter(bool value);
        void setEqualLeft(bool value);
        void setEqualRight(bool value);

        /**
         * Sets string CSTR flags
         * @see flags(), hasFlag()
         */
        void setFlags(uint flag);

        /**
         * Sets string first indent
         * @see firstIndent()
         */
        void setFirstIndent(int indent);

        /**
         * Sets font size penalty for all fonts that more then @b min_size
         */
        void setFontSizePenalty(int min_size, int penalty);

        /**
         * Sets string left indent
         * @see leftIndent(), setRightIndent()
         */
        void setLeftIndent(int indent);

        /**
         * Sets line carrying
         * @see lineCarry()
         */
        void setLineCarry(bool value);

        /**
         * Sets line transfer
         * @see lineTransfer()
         */
        void setLineTransfer(bool value);

        /**
         * Sets string top margin
         * @see marginTop()
         */
        void setTopMargin(int value);

        /**
         * Sets string paragraph begining
         */
        void setParagraphBegin(bool value);

        /**
         * Sets string right indent
         * @see rightIndent(), setLeftIndent()
         */
        void setRightIndent(int indent);

        /**
         * Checks if string starts with symbol chr
         */
        bool startsWith(int c) const;

        /**
         * Checks if string starts with dash
         * @see startsWith()
         */
        bool startsWithDash() const;

        /**
         * Checks if string starts with digit
         * @see startsWith()
         */
        bool startsWithDigit() const;

        /**
         * Returns string content as std::string
         */
        std::string toString() const;

        /**
         * Returns distance between left and right border
         * @return 0 if string is empty
         * @see center(), leftBorder(), rightBorder()
         */
        int width() const;

        /**
         * Returns pointer to word at position @b pos
         * @throw std::out_of_range if no such position
         * @see addWord(), clearWords()
         */
        CRtfWord * wordAt(size_t pos);
        const CRtfWord * wordAt(size_t pos) const;

        /**
         * Returns words count
         * @see empty(), clearWords(), addWord()
         */
        size_t wordCount() const;
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
        bool equal_left_;
        bool equal_right_;
        int first_indent_;
        int left_indent_;
        int right_indent_;
        int margin_top_;
        int real_length_;
        format_align_t align_;
        uint flags_;
};

}

#endif /* CRTFSTRING_H_ */
