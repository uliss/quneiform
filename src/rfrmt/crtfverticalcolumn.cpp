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

#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "creatertf.h"

using namespace CIF;

CRtfVerticalColumn::CRtfVerticalColumn() :
    m_rect(Point(32000, 32000), Point()) {
    m_bSortFlag = 0;
    m_wType = FT_TEXT;
    m_wFragmentsCount = 0;
    m_bFlagObjectInColumn = 0;
    m_PagePtr = 0;
}

CRtfVerticalColumn::~CRtfVerticalColumn() {
    CRtfFragment* cFrag;

    m_wFragmentsCount = m_arFragments.size();
    for (uint32_t i = 0; i < m_wFragmentsCount; i++) {
        cFrag = m_arFragments[i];
        delete cFrag;
    }
}

Bool CRtfVerticalColumn::Write(Bool OutPutType, RtfSectorInfo* SectorInfo) {
    CRtfFragment* pRtfFragment;

    m_wFragmentsCount = m_arFragments.size();
    for (int i = 0; i < m_wFragmentsCount; i++) {
        pRtfFragment = (CRtfFragment*) m_arFragments[i];
        if ((pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE)
                && pRtfFragment->m_bFlagUsed == TRUE)
            continue;
        if (pRtfFragment->m_wType == FT_TABLE) {
            SectorInfo->userNum = pRtfFragment->m_wUserNumberForFormattedMode;
            pRtfFragment->FWriteTable((int) pRtfFragment->m_wUserNumber, SectorInfo, OutPutType);
            pRtfFragment->m_bFlagUsed = TRUE;
        }
        else if (pRtfFragment->m_wType == FT_PICTURE) {
            SectorInfo->userNum = pRtfFragment->m_wUserNumberForFormattedMode;
            pRtfFragment->FWritePicture((int) pRtfFragment->m_wUserNumber, SectorInfo, OutPutType);
            pRtfFragment->m_bFlagUsed = TRUE;
        }
        else {
            if (!pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn
                    && !pRtfFragment->m_RightOffsetFragmentFromVerticalColumn) {
                pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn = pRtfFragment->m_rect.left()
                        - m_rect.left();
                pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right()
                        - pRtfFragment->m_rect.right();
            }
            if (!pRtfFragment->m_WidthVerticalColumn)
                pRtfFragment->m_WidthVerticalColumn = m_rect.width();

            pRtfFragment->pRtfParent = m_PagePtr;
            pRtfFragment->FWriteText(SectorInfo, OutPutType);
        }
    }
    return TRUE;
}

void CRtfVerticalColumn::SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo) {
    CRtfFragment* pRtfFragment;
    Rect LeftFreePlace, RightFreePlace, TopFreePlace, BottomFreePlace, CurrentFragmentRect,
            RectInter;
    int i, CountFragments;
    int32_t LeftFree, RightFree, TopFree, BottomFree;

    LeftFree = CurrentFragment->m_rect.left();
    RightFree = SectorInfo->PaperW - CurrentFragment->m_rect.right();
    TopFree = CurrentFragment->m_rect.top();
    BottomFree = SectorInfo->PaperH - CurrentFragment->m_rect.bottom();

    LeftFreePlace.rleft() = 0;
    LeftFreePlace.rright() = MAX(0, CurrentFragment->m_rect.left() - 1);
    LeftFreePlace.rtop() = CurrentFragment->m_rect.top();
    LeftFreePlace.rbottom() = CurrentFragment->m_rect.bottom();

    RightFreePlace.rleft() = CurrentFragment->m_rect.right() + 1;
    RightFreePlace.rright() = SectorInfo->PaperW;
    RightFreePlace.rtop() = CurrentFragment->m_rect.top();
    RightFreePlace.rbottom() = CurrentFragment->m_rect.bottom();

    TopFreePlace.rleft() = CurrentFragment->m_rect.left();
    TopFreePlace.rright() = CurrentFragment->m_rect.right();
    TopFreePlace.rtop() = 0;
    TopFreePlace.rbottom() = MAX(0, CurrentFragment->m_rect.top() - 1);

    BottomFreePlace.rleft() = CurrentFragment->m_rect.left();
    BottomFreePlace.rright() = CurrentFragment->m_rect.right();
    BottomFreePlace.rtop() = CurrentFragment->m_rect.bottom() + 1;
    BottomFreePlace.rbottom() = SectorInfo->PaperH;

    CountFragments = m_PagePtr->m_arFragments.size();
    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = m_PagePtr->m_arFragments[i];
        if (pRtfFragment->m_wType == FT_PICTURE || pRtfFragment->m_wType == FT_TABLE)
            continue;

        CurrentFragmentRect = pRtfFragment->m_rect;

        if (CurrentFragmentRect.intersects(LeftFreePlace))
            LeftFree = MIN(LeftFree, LeftFreePlace.right() - CurrentFragmentRect.right());

        if (CurrentFragmentRect.intersects(RightFreePlace))
            RightFree = MIN(RightFree, CurrentFragmentRect.left() - RightFreePlace.left());

        if (CurrentFragmentRect.intersects(TopFreePlace))
            TopFree = MIN(TopFree, TopFreePlace.bottom() - CurrentFragmentRect.bottom());

        if (CurrentFragmentRect.intersects(BottomFreePlace))
            BottomFree = MIN(BottomFree, CurrentFragmentRect.top() - BottomFreePlace.top());
    }

    SectorInfo->m_rectFree.rleft() = CurrentFragment->m_rectFree.rleft() = (int32_t) (MAX(0,
            LeftFree) * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.rright() = CurrentFragment->m_rectFree.rright() = (int32_t) (MAX(0,
            RightFree) * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.rtop() = CurrentFragment->m_rectFree.rtop() = (int32_t) (MAX(0, TopFree)
            * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.rbottom() = CurrentFragment->m_rectFree.rbottom() = (int32_t) (MAX(0,
            BottomFree) * TwipsToEMU_Koef);

}
