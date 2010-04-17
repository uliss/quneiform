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

#include "rtfword.h"
#include "crtfchar.h"
#include "creatertf.h"
#include "cpage/cpage.h"
#include "minmax.h"

CRtfWord::CRtfWord() {
    m_wCharsCount = 0;
}

CRtfWord::~CRtfWord() {
    CRtfChar* cChar;
    m_wCharsCount = m_arChars.size();

    for (int i = 0; i < m_wCharsCount; i++) {
        cChar = m_arChars[i];
        delete cChar;
    }
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
    pRtfCharFirst = (CRtfChar*) m_arChars[0];
    pRtfCharLast = (CRtfChar*) m_arChars[m_wCharsCount - 1];
    m_wcl = (int16_t) pRtfCharFirst->m_Realrect.left;
    m_wcr = (int16_t) pRtfCharLast->m_Realrect.right;
    m_wct = MIN((int16_t) pRtfCharFirst->m_Realrect.top, (int16_t) pRtfCharLast->m_Realrect.top);
    m_wcb = MAX((int16_t) pRtfCharFirst->m_Realrect.bottom,
            (int16_t) pRtfCharLast->m_Realrect.bottom);

    for (nz = 0; nz < m_wCharsCount; nz++) {
        pRtfChar = (CRtfChar*) m_arChars[nz];
        m_wcp = MIN(m_wcp, pRtfChar->m_chrVersions[0].m_bProbability);
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

