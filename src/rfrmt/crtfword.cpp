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

#include "crtfword.h"
#include "crtfchar.h"
#include "creatertf.h"
#include "cpage/cpage.h"
#include "minmax.h"

namespace CIF
{

CRtfWord::CRtfWord() {
}

CRtfWord::~CRtfWord() {
    clearChars();
}

void CRtfWord::addChar(CRtfChar * chr) {
    chars_.push_back(chr);
}

CRtfChar * CRtfWord::charAt(size_t pos) {
    return chars_.at(pos);
}

size_t CRtfWord::charCount() const {
    return chars_.size();
}

void CRtfWord::clearChars() {
    for (CharList::iterator it = chars_.begin(), end = chars_.end(); it != end; ++it)
        delete *it;
    chars_.clear();
}

CRtfChar * CRtfWord::firstChar() {
    return chars_.front();
}

font_number CRtfWord::fontNumber() const {
    return font_number_;
}

short CRtfWord::idealFontSize() const {
    return m_wIdealFontPointSize;
}

CRtfChar * CRtfWord::lastChar() {
    return chars_.back();
}

short CRtfWord::realFontSize() const {
    return m_wRealFontPointSize;
}

void CRtfWord::setFontNumber(font_number number) {
    font_number_ = number;
}

void CRtfWord::getCoordinatesAndProbability() {
    int16_t nz;
    int16_t t, l, b, r;
    CRtfChar *pRtfCharFirst, *pRtfCharLast;
    PAGEINFO PageInfo;
    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    GetPageInfo(hCPAGE, &PageInfo);
    m_wcl = m_wct = 32000;
    m_wcr = m_wcb = 0;
    m_wcs = 1;
    m_wcp = 254;
    pRtfCharFirst = chars_.front();
    pRtfCharLast = chars_.back();

    m_wcl = (int16_t) pRtfCharFirst->realRect().left();
    m_wcr = (int16_t) pRtfCharLast->realRect().right();
    m_wct = MIN(pRtfCharFirst->realRect().top(), pRtfCharLast->realRect().top());
    m_wcb = MAX(pRtfCharFirst->realRect().bottom(), pRtfCharLast->realRect().bottom());

    for (nz = 0; nz < chars_.size(); nz++) {
        CRtfChar * pRtfChar = chars_[nz];
        m_wcp = MIN(m_wcp, pRtfChar->first().probability());
        m_wcs = MIN(m_wcs, pRtfChar->m_bFlg_spell);
    }

    if (PageInfo.Angle) {
        t = m_wct;
        r = m_wcr;
        b = m_wcb;
        l = m_wcl;

        if (PageInfo.Angle == 90) { //270
            m_wcl = t;
            m_wcr = b;
            m_wct = (int16_t) PageInfo.Height - r;
            m_wcb = (int16_t) PageInfo.Height - l;
        }

        else if (PageInfo.Angle == 270) { //90
            m_wcl = (int16_t) PageInfo.Width - b;
            m_wcr = (int16_t) PageInfo.Width - t;
            m_wct = l;
            m_wcb = r;
        }

        else if (PageInfo.Angle == 180) {
            m_wcl = (int16_t) PageInfo.Width - r;
            m_wcr = (int16_t) PageInfo.Width - l;
            m_wct = (int16_t) PageInfo.Height - b;
            m_wcb = (int16_t) PageInfo.Height - t;
        }
    }
}

void CRtfWord::setIdealFontSize(short size) {
    m_wIdealFontPointSize = size;
}

void CRtfWord::setRealFontSize(short size) {
    m_wRealFontPointSize = size;
}

}

