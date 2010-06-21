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

#include "crtfsector.h"
#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtffunc.h"
#include "ced/ced.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "minmax.h"

namespace CIF
{

RfrmtDrawSectorFunction CRtfSector::draw_func_;

CRtfSector::CRtfSector() {
    m_bFlagLine = FALSE;
    m_FlagOneString = FALSE;
    SectorInfo.Offset.rx() = DefMargL / 2;
    SectorInfo.Offset.ry() = 32000;
    SectorInfo.CountFragments = 0;
    SectorInfo.InterSectorDist = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
    SetRect(&m_rectReal, 32000, 32000, 0, 0);
}

CRtfSector::~CRtfSector() {
    clearColumns();
}

void CRtfSector::addColumn(CRtfHorizontalColumn * col) {
    hcols_.push_back(col);
}

void CRtfSector::clearColumns() {
    for (iterator it = hcols_.begin(), end = hcols_.end(); it != end; ++it)
        delete *it;

    hcols_.clear();
}

size_t CRtfSector::columnCount() const {
    return hcols_.size();
}

void CRtfSector::setDrawCallback(RfrmtDrawSectorFunction f) {
    draw_func_ = f;
}

void CRtfSector::setPage(CRtfPage * page) {
    page_ = page;
}

void CRtfSector::calcSector() {
    for (size_t i = 0; i < hcols_.size(); i++) {
        CRtfHorizontalColumn * hcol = hcols_[i];
        hcol->calcHorizontalColumn();

        if (hcol->type() < CRtfHorizontalColumn::ALL_FRAME)
            terminal_col_idx_.push_back(i);
    }
}

Bool CRtfSector::Write() {
    EDBOX playout;
    EDSIZE interval;

    if (m_bFlagLine == FALSE) {
        FillingSectorInfo();
        //FRAMES привязанные к началу сектора (это работает только когда сектор целиком состоит из frames)
        WriteNonTerminalColumns();
        // каждая таблица может иметь leftOffs & topOffset
        WriteTerminalColumnsTablesAndPictures();
        //это настоящие (не Frames) колонки - каждая колонки может иметь leftOffs & topOffset of section
        WriteTerminalColumns();
    }

    Rect indent;
    interval.cx = 0;
    interval.cy = SectorInfo.InterSectorDist;
    playout.x = -1;
    playout.w = -1;
    playout.y = -1;
    playout.h = -1;
    CEDParagraph * par = CED_CreateParagraph(SectorInfo.hEDSector, SectorInfo.hColumn, ALIGN_LEFT,
            indent, SectorInfo.userNum, -1, interval, playout, Color::null(), -1, -1, -1, FALSE);

    if (m_bFlagLine == TRUE) {
        CED_SetParaBorders(par, 0, 0, 0, 0, 1, 20, 0, 0, TRUE);
    }

    // FIXME uliss: using hardcoded font size
    CEDLine * ln = new CEDLine(false, 6);
    par->addLine(ln);
    return TRUE;
}

void CRtfSector::WriteTerminalColumnsTablesAndPictures() {
    int32_t CountHTerminalColumns, TerminalColumnNumber;
    CRtfHorizontalColumn *pRtfHorizontalColumn;
    CountHTerminalColumns = terminal_col_idx_.size();

    for (int i = 0; i < CountHTerminalColumns; i++) {
        TerminalColumnNumber = terminal_col_idx_[i];
        pRtfHorizontalColumn = hcols_[TerminalColumnNumber];
        pRtfHorizontalColumn->setPage(page_);
        pRtfHorizontalColumn->writeTablesAndPictures(&SectorInfo);
    }
}

void CRtfSector::WriteTerminalColumns() {
    int CountHTerminalColumns = terminal_col_idx_.size();
    m_VTerminalColumnNumber = 0;
    m_arRightBoundTerminalColumns.clear();
    m_arWidthTerminalColumns.clear();

    if (CountHTerminalColumns > 0) {
        int CountVTerminalColumns = GetCountAndRightBoundVTerminalColumns();

        for (int i = 0; i < CountHTerminalColumns; i++) {
            int TerminalColumnNumber = terminal_col_idx_[i];
            CRtfHorizontalColumn * hcol = hcols_[TerminalColumnNumber];
            SectorInfo.VerticalOffsetColumnFromSector = (uint16_t) (hcol->m_rect.top - m_rect.top);
            hcol->setPage(page_);
            hcol->writeTerminalColumns(&m_arRightBoundTerminalColumns, &m_VTerminalColumnNumber,
                    CountVTerminalColumns, &SectorInfo);
        }
    }
}

int32_t CRtfSector::GetCountAndRightBoundVTerminalColumns(void) {
    int32_t CountHTerminalColumns, CountVTerminalColumns, TerminalColumnNumber;
    CRtfHorizontalColumn *pRtfHorizontalColumn;
    CountVTerminalColumns = 0;
    CountHTerminalColumns = terminal_col_idx_.size();

    for (int i = 0; i < CountHTerminalColumns; i++) {
        TerminalColumnNumber = terminal_col_idx_[i];
        pRtfHorizontalColumn = hcols_[TerminalColumnNumber];
        CountVTerminalColumns += pRtfHorizontalColumn->getCountAndRightBoundVTerminalColumns(
                &m_arRightBoundTerminalColumns, &m_arWidthTerminalColumns);
    }

    return CountVTerminalColumns;
}

void CRtfSector::WriteNonTerminalColumns() {
    SectorInfo.FlagFictiveParagraph = TRUE;

    for (size_t i = 0; i < hcols_.size(); i++) {
        CRtfHorizontalColumn * hcol = hcols_[i];

        if (hcol->type() >= CRtfHorizontalColumn::ALL_FRAME)
            hcol->writeNonTerminalColumns(&SectorInfo);
    }
}

void CRtfSector::ToPlacePicturesAndTables(CRtfFragment* frag) {
    CRtfHorizontalColumn * hcol;

    if (hcols_.empty()) {
        hcols_.push_back(new CRtfHorizontalColumn);
        hcol = hcols_.back();
        hcol->ToPlacePicturesAndTables(frag);
        return;
    }

    hcol = hcols_.back();

    if (frag->m_rect.left >= hcol->m_rectReal.right) {
        hcols_.push_back(new CRtfHorizontalColumn());
        hcol = hcols_.back();
        hcol->ToPlacePicturesAndTables(frag);
        return;
    }

    for (size_t j = 0; j < hcols_.size(); j++) {
        hcol = hcols_[j];

        if (frag->m_rect.right <= hcol->m_rectReal.left) {
            hcol = *hcols_.insert(hcols_.begin() + j, new CRtfHorizontalColumn());
            hcol->ToPlacePicturesAndTables(frag);
            return;
        } else {
            if ((frag->m_rect.left < hcol->m_rectReal.left && frag->m_rect.right
                    > hcol->m_rectReal.left) || (frag->m_rect.left >= hcol->m_rectReal.left
                    && frag->m_rect.left < hcol->m_rectReal.right)) {
                hcol->ToPlacePicturesAndTables(frag);
                return;
            }
        }
    }
}

//~ расчет размеров сектора
void CRtfSector::FillingSectorInfo() //~ тут происходит работа со старой (vmk) структурой колонок
{
    CRtfHorizontalColumn *pRtfHorizontalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    int CountVColumn, CountFragments;

    if (!terminal_col_idx_.empty()) {
        pRtfHorizontalColumn = hcols_[terminal_col_idx_[0]];

        if (m_FlagOneString == FALSE)
            SectorInfo.Offset.rx() = MAX(pRtfHorizontalColumn->m_rect.left, 0);
    }

    for (size_t i = 0; i < hcols_.size(); i++) {
        pRtfHorizontalColumn = hcols_[i];
        CountVColumn = pRtfHorizontalColumn->columnCount();

        for (int i1 = 0; i1 < CountVColumn; i1++) {
            pRtfVerticalColumn = pRtfHorizontalColumn->columnAt(i1);
            pRtfVerticalColumn->setPage(page_);
            CountFragments = pRtfVerticalColumn->fragmentCount();

            for (int i2 = 0; i2 < CountFragments; i2++) {
                SectorInfo.CountFragments++;
                pRtfFragment = pRtfVerticalColumn->fragmentAt(i2);
                //!!!Art   if(pRtfFragment->type_ == FT_FRAME || pRtfFragment->type_ == FT_TEXT )
                //!!!Art   SectorInfo.Offset.y = MIN(SectorInfo.Offset.y, pRtfFragment->m_rect.top);
            }
        }
    }
}

}

