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

#include "crtfsector.h"
#include "crtffragment.h"
#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "ced/ced.h"
#include "creatertf.h"

using namespace CIF;

CRtfSector::CRtfSector() :
    m_rect(Point(32000, 32000), Point()), m_rectReal(Point(32000, 32000), Point()) {
    m_bFlagLine = FALSE;
    m_wHorizontalColumnsCount = 0;
    m_FlagOneString = FALSE;
    SectorInfo.Offset.rx() = DefMargL / 2;
    SectorInfo.Offset.ry() = 32000;
    SectorInfo.CountFragments = 0;
    SectorInfo.InterSectorDist = 0;
}

CRtfSector::~CRtfSector() {
    CRtfHorizontalColumn* cHorizontalColumn;
    m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
    for (int i = 0; i < m_wHorizontalColumnsCount; i++) {
        cHorizontalColumn = m_arHorizontalColumns[i];
        delete cHorizontalColumn;
    }
}

void CRtfSector::CalcSector(void) {
    CRtfHorizontalColumn* pRtfHorizontalColumn;
    m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
    for (int ih = 0; ih < m_wHorizontalColumnsCount; ih++) {
        pRtfHorizontalColumn = m_arHorizontalColumns[ih];
        pRtfHorizontalColumn->CalcHorizontalColumn();
        if (pRtfHorizontalColumn->m_wType < HC_AllFrame)
            m_arHTerminalColumnsIndex.push_back(ih);
    }
}

Bool CRtfSector::Write(void) {
#ifdef EdWrite
    Handle hParagraph = NULL;
    Handle hString = NULL;
    Rect indent;
    EDBOX playout;
    Size interval;
    int align;
#endif

    if (m_bFlagLine == FALSE) {
        FillingSectorInfo();
        WriteNonTerminalColumns(); //FRAMES привязанные к началу сектора (это работает только когда сектор целиком состоит из frames)
        WriteTerminalColumnsTablesAndPictures(); // каждая таблица может иметь leftOffs & topOffset
        WriteTerminalColumns(); //это настоящие (не Frames) колонки - каждая колонки может иметь leftOffs & topOffset of section
        Put("{\\pard\\fs6");
        PutCom("\\sa", SectorInfo.InterSectorDist, 0);
        Put("\\par}");
    }
    else {
        Put("{\\pard\\plain\\nowidctlpar\\brdrt\\brdrs\\brdrw15\\adjustright\\fs6");
        PutCom("\\sa", SectorInfo.InterSectorDist, 0);
        Put("\\par}");
    }

    if (RtfWriteMode)
        return TRUE;

#ifdef EdWrite
    indent = Rect();
    interval.rwidth() = 0;
    interval.rheight() = SectorInfo.InterSectorDist;
    playout.x = -1;
    playout.w = -1;
    playout.y = -1;
    playout.h = -1;

    align = TP_LEFT_ALLIGN;
    hParagraph = CED_CreateParagraph(SectorInfo.hEDSector, SectorInfo.hColumn, align, indent,
            SectorInfo.userNum, -1, interval, playout, -1, /*0*/-1, -1, -1, FALSE);
    if (m_bFlagLine == TRUE) {
        CED_SetParaBorders(hParagraph, 0, 0, 0, 0, 1, 20, 0, 0, TRUE);
    }
    hString = CED_CreateLine(hParagraph, FALSE, 6);
#endif

    return TRUE;
}

void CRtfSector::WriteTerminalColumnsTablesAndPictures(void) {
    int32_t CountHTerminalColumns, TerminalColumnNumber;
    CRtfHorizontalColumn *pRtfHorizontalColumn;

    CountHTerminalColumns = m_arHTerminalColumnsIndex.size();

    for (int i = 0; i < CountHTerminalColumns; i++) {
        TerminalColumnNumber = m_arHTerminalColumnsIndex[i];
        pRtfHorizontalColumn = m_arHorizontalColumns[TerminalColumnNumber];
        pRtfHorizontalColumn->m_PagePtr = this->m_PagePtr;
        pRtfHorizontalColumn->WriteTerminalColumnsTablesAndPictures(&SectorInfo);
    }
}

void CRtfSector::WriteTerminalColumns(void) {
    int32_t CountHTerminalColumns, CountVTerminalColumns, TerminalColumnNumber;
    CRtfHorizontalColumn *pRtfHorizontalColumn;

    CountHTerminalColumns = m_arHTerminalColumnsIndex.size();
    m_VTerminalColumnNumber = 0;
    m_arRightBoundTerminalColumns.clear();
    m_arWidthTerminalColumns.clear();

    if (CountHTerminalColumns > 0) {
        CountVTerminalColumns = GetCountAndRightBoundVTerminalColumns();
        PutCom("\\cols", CountVTerminalColumns, 0);
        int32_t len_colsx = 709;
        PutCom("\\colsx", len_colsx, 1);
        Put("\\endnhere");

        for (int i = 0; i < CountHTerminalColumns; i++) {
            TerminalColumnNumber = m_arHTerminalColumnsIndex[i];
            pRtfHorizontalColumn = m_arHorizontalColumns[TerminalColumnNumber];
            SectorInfo.VerticalOffsetColumnFromSector
                    = (uint16_t) (pRtfHorizontalColumn->m_rect.top() - m_rect.top());
            pRtfHorizontalColumn->m_PagePtr = this->m_PagePtr;
            pRtfHorizontalColumn->WriteTerminalColumns(&m_arRightBoundTerminalColumns,
                    &m_VTerminalColumnNumber, CountVTerminalColumns, &SectorInfo);
        }
    }
}

int32_t CRtfSector::GetCountAndRightBoundVTerminalColumns(void) {
    int32_t CountHTerminalColumns, CountVTerminalColumns, TerminalColumnNumber;
    CRtfHorizontalColumn *pRtfHorizontalColumn;

    CountVTerminalColumns = 0;
    CountHTerminalColumns = m_arHTerminalColumnsIndex.size();
    for (int i = 0; i < CountHTerminalColumns; i++) {
        TerminalColumnNumber = m_arHTerminalColumnsIndex[i];
        pRtfHorizontalColumn = m_arHorizontalColumns[TerminalColumnNumber];
        CountVTerminalColumns += pRtfHorizontalColumn->GetCountAndRightBoundVTerminalColumns(
                &m_arRightBoundTerminalColumns, &m_arWidthTerminalColumns);
    }
    return CountVTerminalColumns;
}

void CRtfSector::WriteNonTerminalColumns(void) {
    CRtfHorizontalColumn *pRtfHorizontalColumn;

    SectorInfo.FlagFictiveParagraph = TRUE;

    m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
    for (int i = 0; i < m_wHorizontalColumnsCount; i++) {
        pRtfHorizontalColumn = m_arHorizontalColumns[i];
        if (pRtfHorizontalColumn->m_wType >= HC_AllFrame)
            pRtfHorizontalColumn->WriteNonTerminalColumns(&SectorInfo);
    }
}

void CRtfSector::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment) {
    CRtfHorizontalColumn *pRtfHorizontalColumn;

    m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
    if (m_wHorizontalColumnsCount == 0) {
        m_arHorizontalColumns.push_back(new CRtfHorizontalColumn());
        pRtfHorizontalColumn = m_arHorizontalColumns.back();
        pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
        return;
    }

    pRtfHorizontalColumn = m_arHorizontalColumns.back();
    if (pRtfFragment->m_rect.left() >= pRtfHorizontalColumn->m_rectReal.right()) {
        m_arHorizontalColumns.push_back(new CRtfHorizontalColumn());
        pRtfHorizontalColumn = m_arHorizontalColumns.back();
        pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
        return;
    }

    for (int j = 0; j < m_wHorizontalColumnsCount; j++) {
        pRtfHorizontalColumn = m_arHorizontalColumns[j];
        if (pRtfFragment->m_rect.right() <= pRtfHorizontalColumn->m_rectReal.left()) {
            pRtfHorizontalColumn = *m_arHorizontalColumns.insert(m_arHorizontalColumns.begin() + j,
                    new CRtfHorizontalColumn());
            pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
            return;
        }
        else {
            if ((pRtfFragment->m_rect.left() < pRtfHorizontalColumn->m_rectReal.left()
                    && pRtfFragment->m_rect.right() > pRtfHorizontalColumn->m_rectReal.left()) ||

            (pRtfFragment->m_rect.left() >= pRtfHorizontalColumn->m_rectReal.left()
                    && pRtfFragment->m_rect.left() < pRtfHorizontalColumn->m_rectReal.right())) {
                pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
                return;
            }
        }
    }

}
//~ расчет размеров сектора
void CRtfSector::FillingSectorInfo() { //~ тут происходит работа си старой (vmk) структурой колонок
    CRtfHorizontalColumn *pRtfHorizontalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    int CountVColumn, CountFragments;

    m_wHorizontalColumnsCount = m_arHTerminalColumnsIndex.size();
    if (m_wHorizontalColumnsCount) {
        pRtfHorizontalColumn = m_arHorizontalColumns[m_arHTerminalColumnsIndex[0]];
        if (m_FlagOneString == FALSE)
            SectorInfo.Offset.rx() = MAX(pRtfHorizontalColumn->m_rect.left(), 0);
    }

    m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
    for (int i = 0; i < m_wHorizontalColumnsCount; i++) {
        pRtfHorizontalColumn = m_arHorizontalColumns[i];
        CountVColumn = pRtfHorizontalColumn->m_arVerticalColumns.size();
        for (int i1 = 0; i1 < CountVColumn; i1++) {
            pRtfVerticalColumn = pRtfHorizontalColumn->m_arVerticalColumns[i1];
            pRtfVerticalColumn->m_PagePtr = m_PagePtr;
            CountFragments = pRtfVerticalColumn->m_arFragments.size();
            for (int i2 = 0; i2 < CountFragments; i2++) {
                SectorInfo.CountFragments++;
                pRtfFragment = pRtfVerticalColumn->m_arFragments[i2];

                //!!!Art   if(pRtfFragment->m_wType == FT_FRAME || pRtfFragment->m_wType == FT_TEXT )
                //!!!Art   SectorInfo.Offset.y = MIN(SectorInfo.Offset.y, pRtfFragment->m_rect.top());
            }
        }
    }
}
