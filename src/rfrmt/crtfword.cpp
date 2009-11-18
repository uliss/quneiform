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
    m_wCharsCount = 0;
}

CRtfWord::~CRtfWord() {
    for (vectorChar::iterator it = m_arChars.begin(), end = m_arChars.end(); it != end; ++it)
        delete *it;
}

CRtfChar* CRtfWord::GetNextChar() {
    m_arChars.push_back(new CRtfChar());
    return m_arChars.back();
}

void CRtfWord::get_coordinates_and_probability() {
    int16_t nz;
    int16_t t, l, b, r;

    CRtfChar *pRtfChar, *pRtfCharFirst, *pRtfCharLast;
    PAGEINFO PageInfo;

    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    GetPageInfo(hCPAGE, &PageInfo);

    m_wcl = m_wct = 32000;
    m_wcr = m_wcb = 0;
    m_wcs = 1;
    m_wcp = 254;

    pRtfCharFirst = m_arChars.front();
    pRtfCharLast = m_arChars.back();

    m_wcl = pRtfCharFirst->real_rect_.left();
    m_wcr = pRtfCharLast->real_rect_.right();
    m_wct = std::min(pRtfCharFirst->real_rect_.top(), pRtfCharLast->real_rect_.top());
    m_wcb = std::max(pRtfCharFirst->real_rect_.bottom(), pRtfCharLast->real_rect_.bottom());

    m_wCharsCount = m_arChars.size();
    for (nz = 0; nz < m_wCharsCount; nz++) {
        pRtfChar = m_arChars[nz];
        m_wcp = std::min((int) m_wcp, (int) pRtfChar->versions[0].probability_);
        m_wcs = std::min((int) m_wcs, (int) pRtfChar->m_bFlg_spell);
    }

    if (PageInfo.Angle) {
        t = m_wct;
        r = m_wcr;
        b = m_wcb;
        l = m_wcl;
        if (PageInfo.Angle == 90) //270
        {
            m_wcl = t;
            m_wcr = b;
            m_wct = (int16_t) PageInfo.Height - r;
            m_wcb = (int16_t) PageInfo.Height - l;
        }
        else if (PageInfo.Angle == 270) //90
        {
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
