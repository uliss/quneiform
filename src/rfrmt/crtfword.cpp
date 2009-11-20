/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <algorithm>

#include "crtfword.h"
#include "cpage/cpage.h"

CRtfWord::CRtfWord() {
}

CRtfWord::~CRtfWord() {
    for (vectorChar::iterator it = chars.begin(), end = chars.end(); it != end; ++it)
        delete *it;
}

void CRtfWord::calcCoordinates() {
    assert(chars.size() >= 1);
    CRtfChar * pRtfCharFirst = chars.front();
    CRtfChar * pRtfCharLast = chars.back();

    m_wcl = m_wct = 32000;
    m_wcr = m_wcb = 0;

    m_wcl = pRtfCharFirst->real_rect_.left();
    m_wcr = pRtfCharLast->real_rect_.right();
    m_wct = std::min(pRtfCharFirst->real_rect_.top(), pRtfCharLast->real_rect_.top());
    m_wcb = std::max(pRtfCharFirst->real_rect_.bottom(), pRtfCharLast->real_rect_.bottom());

    PAGEINFO PageInfo;
    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    GetPageInfo(hCPAGE, &PageInfo);

    if (PageInfo.Angle) {
        int t = m_wct;
        int r = m_wcr;
        int b = m_wcb;
        int l = m_wcl;
        if (PageInfo.Angle == 90) //270
        {
            m_wcl = t;
            m_wcr = b;
            m_wct = PageInfo.Height - r;
            m_wcb = PageInfo.Height - l;
        }
        else if (PageInfo.Angle == 270) //90
        {
            m_wcl = PageInfo.Width - b;
            m_wcr = PageInfo.Width - t;
            m_wct = l;
            m_wcb = r;
        }
        else if (PageInfo.Angle == 180) {
            m_wcl = PageInfo.Width - r;
            m_wcr = PageInfo.Width - l;
            m_wct = PageInfo.Height - b;
            m_wcb = PageInfo.Height - t;
        }
    }
}

void CRtfWord::getCoordinatesAndProbability() {
    calcCoordinates();

    m_wcs = 1;
    m_wcp = 254;

    for (int nz = 0, size = chars.size(); nz < size; nz++) {
        m_wcp = std::min(m_wcp, chars[nz]->versions[0].probability_);
        m_wcs = std::min(m_wcs, chars[nz]->flag_spell);
    }
}

std::ostream& operator<<(std::ostream& os, const CRtfWord& word) {
    for(size_t i = 0; i < word.chars.size(); i++)
        os << word.chars[i]->versions[0].char_;
    return os;
}
