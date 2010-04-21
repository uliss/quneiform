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

namespace CIF
{

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
         * Removes all words from string
         */
        void clearWords();

        /**
         * Returns pointer to first word
         * @throw std::out_of_range if string is empty
         */
        CRtfWord * firstWord();
        const CRtfWord * firstWord() const;

        /**
         * Returns pointer to last word in string
         * @throw std::out_of_range if string is empty
         */
        CRtfWord * lastWord();
        const CRtfWord * lastWord() const;

        /**
         * Returns max font size among string words
         */
        int maxWordFontSize() const;

        /**
         * Returns real string length in twips
         */
        uint realLength() const;

        /**
         * Returns string content as std::string
         */
        std::string toString() const;

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

        int16_t GetStringSizeInTwips();

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
    private:
        typedef std::vector<CRtfWord*> WordList;
        typedef WordList::iterator WordIterator;
        typedef WordList::const_iterator WordIteratorConst;
        WordList words_;
};

}

#endif /* CRTFSTRING_H_ */
