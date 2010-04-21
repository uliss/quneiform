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

#include <iostream>
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfword.h"
#include "creatertf.h"
#include "crtfstruct.h"
#include "crtffunc.h"
#include "minmax.h"

namespace CIF
{

const int SMALLEST_FONT_SIZE = 3;
const char SPACE = ' ';

CRtfString::CRtfString() {
    m_wLeftIndent = 0;
    m_wRightIndent = 0;
    m_wFirstIndent = 0;
    m_wFlagBeginParagraph = 0;
    m_wAlignment = 0;
    m_wPrevAlignment = 0;
    m_wLeftBorderEqual = 0;
    m_wRightBorderEqual = 0;
    m_wCentreEqual = 0;
    m_bLineTransfer = 0;
    m_FlagCarry = 0;
    S_Flags = 0;
    m_LengthStringInTwips = 0;
}

CRtfString::~CRtfString() {
    clearWords();
}

void CRtfString::addWord(CRtfWord * word) {
    words_.push_back(word);
}

void CRtfString::clearWords() {
    for (WordIterator it = words_.begin(), e = words_.end(); it != e; ++it)
        delete (*it);
    words_.clear();
}

CRtfWord * CRtfString::firstWord() {
    return const_cast<CRtfWord*> (const_cast<const CRtfString*> (this)->firstWord());
}

const CRtfWord * CRtfString::firstWord() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::firstWord] string is empty");
    return words_.front();
}

CRtfWord * CRtfString::lastWord() {
    return const_cast<CRtfWord*> (const_cast<const CRtfString*> (this)->lastWord());
}

const CRtfWord * CRtfString::lastWord() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::lasttWord] string is empty");
    return words_.back();
}

int CRtfString::maxWordFontSize() const {
    int str_max_font = SMALLEST_FONT_SIZE;

    for (size_t i = 0; i < words_.size(); i++)
        str_max_font = MAX(str_max_font, words_[i]->realFontSize());

    return str_max_font;
}

CRtfWord * CRtfString::wordAt(size_t pos) {
    return words_.at(pos);
}

const CRtfWord * CRtfString::wordAt(size_t pos) const {
    return words_.at(pos);
}

size_t CRtfString::wordCount() const {
    return words_.size();
}

int16_t CRtfString::GetStringSizeInTwips() {
    CRtfWord* pRtfWord;
    CRtfChar *pLastChar, *pFirstChar;
    pRtfWord = words_[0];
    pFirstChar = pRtfWord->firstChar();
    pRtfWord = words_.back();
    pLastChar = pRtfWord->lastChar();
    int16_t LenghtStr =
            (int16_t) ((pLastChar->idealRect().right() - pFirstChar->idealRect().left()) * Twips);
    return LenghtStr;
}

uint CRtfString::realLength() const {
    if (words_.empty() || words_.front()->charCount() == 0)
        return 0;

    int16_t strHeight;
    std::string str = toString();
    const CRtfChar *pRtfChar = firstWord()->firstChar();
    return GetRealSize(str.c_str(), str.length(), firstWord()->realFontSize(), pRtfChar->font(),
            &strHeight);
}

std::string CRtfString::toString() const {
    std::string result;

    for (WordIteratorConst it = words_.begin(), e = words_.end(); it != e; ++it) {
        result += (*it)->toString();
        result += SPACE;
    }

    // remove traling space
    if (!result.empty())
        result.erase(result.length() - 1);

    return result;
}

}

