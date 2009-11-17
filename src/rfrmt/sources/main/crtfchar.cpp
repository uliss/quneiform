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

#include "crtfchar.h"
#include "edfile.h"
#include "creatertf.h"

using namespace CIF;

void Rtf_CED_CreateChar(Rect* slayout, letterEx* Letter, CRtfChar* pRtfChar) {
    if (RtfWriteMode)
        return;
    if (pRtfChar) {
        int i;
        slayout->rleft() = pRtfChar->m_Realrect.left() + TemplateOffset.x();
        slayout->rright() = pRtfChar->m_Realrect.right() + TemplateOffset.x();
        slayout->rtop() = pRtfChar->m_Realrect.top() + TemplateOffset.y();
        slayout->rbottom() = pRtfChar->m_Realrect.bottom() + TemplateOffset.y();
        for (i = 0; i < pRtfChar->m_wCountAlt; i++) {
            Letter[i].alternative = pRtfChar->m_chrVersions[i].m_bChar;
            Letter[i].probability = pRtfChar->m_chrVersions[i].m_bProbability | 1;
        }
        Letter[i - 1].probability &= 0xFE;
    }
    else {
        *slayout = Rect(Point(-1, -1), Point(-1, -1));
        Letter->alternative = ' ';
        Letter->probability = 0;
    }
}
