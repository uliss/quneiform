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

#include <cstring>
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfword.h"
#include "creatertf.h"
#include "crtfstruct.h"
#include "minmax.h"

int16_t GetRealSize(char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight);

namespace CIF
{

CRtfString::CRtfString() {
    m_wWordsCount = 0;
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
    CRtfWord* cWord;
    m_wWordsCount = m_arWords.size();

    for (int i = 0; i < m_wWordsCount; i++) {
        cWord = m_arWords[i];
        delete cWord;
    }
}

void CRtfString::addWord(CRtfWord * word) {
    m_arWords.push_back(word);
}

void CRtfString::clearWords() {
    for(WordIterator it = m_arWords.begin(), e= m_arWords.end(); it != e; ++it)
        delete (*it);
    m_arWords.clear();
}

size_t CRtfString::wordCount() const {
    return m_arWords.size();
}

int16_t CRtfString::GetStringSizeInTwips() {
    CRtfWord* pRtfWord;
    CRtfChar *pLastChar, *pFirstChar;
    pRtfWord = (CRtfWord*) m_arWords[0];
    pFirstChar = pRtfWord->firstChar();
    pRtfWord = (CRtfWord*) m_arWords[m_wWordsCount - 1];
    pLastChar = pRtfWord->lastChar();
    int16_t LenghtStr =
            (int16_t) ((pLastChar->idealRect().right() - pFirstChar->idealRect().left()) * Twips);
    return LenghtStr;
}

uint16_t CRtfString::GetRealStringSize() {
    char tmp_str[MAX_BUFFER_SIZE];
    CRtfWord* pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t RealSize;
    int16_t strHeight;
    int index = 0;

    for (int nw = 0; nw < m_wWordsCount; nw++) {
        pRtfWord = (CRtfWord*) m_arWords[nw];
        int CountChars = pRtfWord->charCount();

        for (int nz = 0; nz < CountChars; nz++) {
            pRtfChar = pRtfWord->charAt(nz);
            tmp_str[index++] = pRtfChar->first().getChar();
        }

        tmp_str[index++] = ' ';
    }

    tmp_str[index] = 0;
    pRtfWord = (CRtfWord*) m_arWords[0];
    pRtfChar = pRtfWord->firstChar();
    RealSize = GetRealSize((char*) &tmp_str, strlen(tmp_str), pRtfWord->realFontSize(),
            pRtfChar->font(), &strHeight);
    return RealSize;
}

CRtfWord* CRtfString::GetNextWord() {
    m_arWords.push_back(new CRtfWord());
    return m_arWords.back();
}

uint16_t CRtfString::get_max_font_size() {
    uint16_t nw, str_max_font = 3;
    CRtfWord* pRtfWord;

    for (nw = 0; nw < m_wWordsCount; nw++) {
        pRtfWord = (CRtfWord*) m_arWords[nw];
        str_max_font = MAX(str_max_font, pRtfWord->realFontSize());
    }

    return str_max_font;
}

}

