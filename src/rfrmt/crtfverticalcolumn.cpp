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

#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "creatertf.h"
#include "minmax.h"

CRtfVerticalColumn::CRtfVerticalColumn() {
    m_bSortFlag = 0;
    m_wType = FT_TEXT;
    m_wFragmentsCount = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
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
                pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn = pRtfFragment->m_rect.left
                        - m_rect.left;
                pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right
                        - pRtfFragment->m_rect.right;
            }

            if (!pRtfFragment->m_WidthVerticalColumn)
                pRtfFragment->m_WidthVerticalColumn = (int16_t) (m_rect.right - m_rect.left);

            pRtfFragment->pRtfParent = m_PagePtr;
            pRtfFragment->FWriteText(0, SectorInfo, OutPutType);
        }
    }

    return TRUE;
}

void CRtfVerticalColumn::SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo) {
    CRtfFragment* pRtfFragment;
    RECT LeftFreePlace, RightFreePlace, TopFreePlace, BottomFreePlace, CurrentFragmentRect,
            RectInter;
    int i, CountFragments;
    int32_t LeftFree, RightFree, TopFree, BottomFree;
    LeftFree = CurrentFragment->m_rect.left;
    RightFree = SectorInfo->PaperW - CurrentFragment->m_rect.right;
    TopFree = CurrentFragment->m_rect.top;
    BottomFree = SectorInfo->PaperH - CurrentFragment->m_rect.bottom;
    LeftFreePlace.left = 0;
    LeftFreePlace.right = MAX(0, CurrentFragment->m_rect.left - 1);
    LeftFreePlace.top = CurrentFragment->m_rect.top;
    LeftFreePlace.bottom = CurrentFragment->m_rect.bottom;
    RightFreePlace.left = CurrentFragment->m_rect.right + 1;
    RightFreePlace.right = SectorInfo->PaperW;
    RightFreePlace.top = CurrentFragment->m_rect.top;
    RightFreePlace.bottom = CurrentFragment->m_rect.bottom;
    TopFreePlace.left = CurrentFragment->m_rect.left;
    TopFreePlace.right = CurrentFragment->m_rect.right;
    TopFreePlace.top = 0;
    TopFreePlace.bottom = MAX(0, CurrentFragment->m_rect.top - 1);
    BottomFreePlace.left = CurrentFragment->m_rect.left;
    BottomFreePlace.right = CurrentFragment->m_rect.right;
    BottomFreePlace.top = CurrentFragment->m_rect.bottom + 1;
    BottomFreePlace.bottom = SectorInfo->PaperH;
    CountFragments = m_PagePtr->m_arFragments.size();

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = m_PagePtr->m_arFragments[i];

        if (pRtfFragment->m_wType == FT_PICTURE || pRtfFragment->m_wType == FT_TABLE)
            continue;

        CurrentFragmentRect.left = pRtfFragment->m_rect.left;
        CurrentFragmentRect.right = pRtfFragment->m_rect.right;
        CurrentFragmentRect.top = pRtfFragment->m_rect.top;
        CurrentFragmentRect.bottom = pRtfFragment->m_rect.bottom;

        if (IntersectRect(&RectInter, &CurrentFragmentRect, &LeftFreePlace))
            LeftFree = MIN(LeftFree, LeftFreePlace.right - CurrentFragmentRect.right);

        if (IntersectRect(&RectInter, &CurrentFragmentRect, &RightFreePlace))
            RightFree = MIN(RightFree, CurrentFragmentRect.left - RightFreePlace.left);

        if (IntersectRect(&RectInter, &CurrentFragmentRect, &TopFreePlace))
            TopFree = MIN(TopFree, TopFreePlace.bottom - CurrentFragmentRect.bottom);

        if (IntersectRect(&RectInter, &CurrentFragmentRect, &BottomFreePlace))
            BottomFree = MIN(BottomFree, CurrentFragmentRect.top - BottomFreePlace.top);
    }

    SectorInfo->m_rectFree.left = CurrentFragment->m_rectFree.left = (int32_t) (MAX(0, LeftFree)
            * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.right = CurrentFragment->m_rectFree.right = (int32_t) (MAX(0, RightFree)
            * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.top = CurrentFragment->m_rectFree.top = (int32_t) (MAX(0, TopFree)
            * TwipsToEMU_Koef);
    SectorInfo->m_rectFree.bottom = CurrentFragment->m_rectFree.bottom = (int32_t) (MAX(0,
            BottomFree) * TwipsToEMU_Koef);
}

