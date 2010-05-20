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

#include <boost/bind.hpp>

#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtffunc.h"
#include "crtfpage.h"
#include "rfrmtoptions.h"

#include "common/debug.h"
#include "ced/ced.h"
#include "minmax.h"

namespace CIF
{

CRtfHorizontalColumn::CRtfHorizontalColumn() :
    page_(NULL), type_(SINGLE_TERMINAL) {
    SetRect(&m_rect, 32000, 32000, 0, 0);
    SetRect(&m_rectReal, 32000, 32000, 0, 0);
}

CRtfHorizontalColumn::~CRtfHorizontalColumn() {
    clearColumns();
    clearTerminalColumnsGroup();
    clearTerminalColumnsIndexes();
}

void CRtfHorizontalColumn::addColumn(CRtfVerticalColumn * col) {
    vcols_.push_back(col);
}

void CRtfHorizontalColumn::calcHorizontalColumn() {
    if (type_ == FRAME_AND_COLUMN || type_ == ALL_FRAME) {
        if (checkTermColumn()) {
            type_ = ALL_TERMINAL;
        } else {//~проверка вертикального затенения колонок ("жертва" будет frame)
            findHeadingAndSetFrameFlag(); //это проверка, что после удаления жертвы все стало лучше
            defineTerminalProperty(); //присвоение признака терминальности колонкам
        }
    }

    if (type_ <= FRAME_AND_COLUMN)
        fillVTerminalColumnsIndex(); //есть хорошие колонки
}

void addBigColToHist(const CRtfVerticalColumn * col, Histogram& hist, int left_border) {
    assert(col);

    if ((col->type() == FT_TEXT || col->type() == FT_FRAME) && !col->isSmall()) {
        const int col_left = col->m_rectReal.left - left_border;
        const int col_right = col->m_rectReal.right - left_border;

        assert(hist.size() < col_right);

        for (int i = col_left; i < col_right; i++)
            hist[i]++;
    }
}

void CRtfHorizontalColumn::accumulateHistorgam(Histogram& hist, int left_border) const {
    std::for_each(vcols_.begin(), vcols_.end(), boost::bind(addBigColToHist, _1, hist, left_border));
}

bool CRtfHorizontalColumn::checkTermColumn() const {
    for (size_t i = 1; i < vcols_.size(); i++) {
        const CRtfVerticalColumn * vcol = vcols_[i];
        const CRtfVerticalColumn * vcol_prev = vcols_[i - 1];

        if (vcol->m_rectReal.top < vcol_prev->m_rectReal.bottom)
            return false;
    }

    return true;
}

void CRtfHorizontalColumn::clearColumns() {
    for (VColumnIterator it = vcols_.begin(), end = vcols_.end(); it != end; ++it)
        delete *it;

    vcols_.clear();
}

void CRtfHorizontalColumn::clearTerminalColumnsGroup() {
    for (VectorWordIterator it = terminal_col_group_.begin(), end = terminal_col_group_.end(); it
            != end; ++it) {
        delete *it;
    }

    terminal_col_group_.clear();
}

void CRtfHorizontalColumn::clearTerminalColumnsIndexes() {
    for (VectorWordIterator it = terminal_col_idx_.begin(), end = terminal_col_idx_.end(); it
            != end; ++it) {
        delete *it;
    }
    terminal_col_idx_.clear();
}

CRtfVerticalColumn * CRtfHorizontalColumn::columnAt(size_t pos) {
    return vcols_.at(pos);
}

const CRtfVerticalColumn * CRtfHorizontalColumn::columnAt(size_t pos) const {
    return vcols_.at(pos);
}

size_t CRtfHorizontalColumn::columnCount() const {
    return vcols_.size();
}

void CRtfHorizontalColumn::setPage(CRtfPage * page) {
    page_ = page;
}

void CRtfHorizontalColumn::setType(column_t type) {
    type_ = type;
}

CRtfHorizontalColumn::column_t CRtfHorizontalColumn::type() const {
    return type_;
}

int CRtfHorizontalColumn::leftBigVColumnBorder() const {
    int left = 0;

    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * vcol = vcols_[i];

        if ((vcol->type() == FT_TEXT || vcol->type() == FT_FRAME) && !vcol->isSmall()) {
            if (i == 0)
                left = vcol->m_rectReal.left;
            else
                left = std::min(left, (int) vcol->m_rectReal.left);
        }
    }

    return left;
}

void determineColSize(CRtfVerticalColumn * col, int threshold_width, int threshold_height) {
    if (col->type() == FT_TEXT || col->type() == FT_FRAME) {
        if ((col->realWidth() < threshold_width) && (col->realHeight() < threshold_height)) {
            col->setSmall(true);
            col->setType(FT_FRAME);
        } else
            col->setSmall(false);
    }
}

void CRtfHorizontalColumn::markSmallColumns() {
    const int threshold_width = maxVColumnWidth() / 2;
    const int threshold_height = maxVColumnHeight() / 2;

    // all small column became frames
    std::for_each(vcols_.begin(), vcols_.end(), boost::bind(determineColSize, _1, threshold_width,
            threshold_height));
}

int CRtfHorizontalColumn::maxVColumnHeight() const {
    int max_height = 1;

    for (VColumnIteratorConst it = vcols_.begin(), end = vcols_.end(); it != end; ++it) {
        if ((*it)->type() == FT_TEXT || (*it)->type() == FT_FRAME)
            max_height = std::max(max_height, (*it)->realHeight());
    }

    return max_height;
}

int CRtfHorizontalColumn::maxVColumnWidth() const {
    int max_width = 1;

    for (VColumnIteratorConst it = vcols_.begin(), end = vcols_.end(); it != end; ++it) {
        if ((*it)->type() == FT_TEXT || (*it)->type() == FT_FRAME)
            max_width = std::max(max_width, (*it)->realWidth());
    }

    return max_width;
}

void CRtfHorizontalColumn::processColsByHist(const Histogram& hist, int left_offset) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * vcol = vcols_[i];

        if ((vcol->type() == FT_TEXT || vcol->type() == FT_FRAME) && !vcol->isSmall()) {
            const int left = vcol->m_rectReal.left - left_offset;
            const int right = vcol->m_rectReal.right - left_offset;

            if (hist.findU(left, right))
                vcol->setType(FT_FRAME);
            else
                vcol->setType(FT_TEXT);
        }
    }
}

int CRtfHorizontalColumn::rightBigVColumnBorder() const {
    int right = 0;

    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * vcol = vcols_[i];

        if ((vcol->type() == FT_TEXT || vcol->type() == FT_FRAME) && !vcol->isSmall()) {
            if (i == 0)
                right = vcol->m_rectReal.right;
            else
                right = std::min(right, (int) vcol->m_rectReal.right);
        }
    }

    return right;
}

void CRtfHorizontalColumn::processSpaceByHist(const Histogram& hist) {
    hist_spaces_.clear();
    hist.spacePosition(std::back_inserter(hist_spaces_));
}

//после удаления жертвы по гистограммам проверяем разделимость остальных колонок и если да,
// все они будут терминал.
void CRtfHorizontalColumn::findHeadingAndSetFrameFlag() {
    markSmallColumns();
    const int Left = leftBigVColumnBorder();
    const int Right = rightBigVColumnBorder();
    assert(Right - Left >= 0);
    Histogram Hist(Right - Left);

    accumulateHistorgam(Hist, Left);
    processColsByHist(Hist, Left);
}

void CRtfHorizontalColumn::defineTerminalProperty() {
    VectorWord* pGroup;
    CRtfVerticalColumn* pRtfVerticalColumn;
    int32_t Left = 32000, Right = 0, Length, Left1, Right1;
    int i, j, FlagSpace = 0, CountColumn, MinLeft, MaxRight;

    for (size_t i = 0; i < vcols_.size(); i++) {
        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_TEXT) {
            Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left);
            Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
        }
    }

    MinLeft = Left;
    MaxRight = Right;
    Length = Right - Left;

    Histogram Hist(Length);

    for (size_t i = 0; i < vcols_.size(); i++) {
        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_TEXT) {
            Left1 = pRtfVerticalColumn->m_rectReal.left - Left;
            Right1 = pRtfVerticalColumn->m_rectReal.right - Left;

            for (j = Left1; j < Right1; j++)
                Hist[j]++;
        }
    }

    processSpaceByHist(Hist);

    CountColumn = hist_spaces_.size();

    if (CountColumn) {
        for (i = 0; i <= CountColumn; i++) {
            terminal_col_group_.push_back(new VectorWord);
            pGroup = terminal_col_group_[i];

            if (i == 0) {
                Left = MinLeft;
                Right = MinLeft + hist_spaces_[i];
            } else if (i == CountColumn) {
                Left = MinLeft + hist_spaces_[i - 1];
                Right = MaxRight;
            } else {
                Left = MinLeft + hist_spaces_[i - 1];
                Right = MinLeft + hist_spaces_[i];
            }

            for (size_t j = 0; j < vcols_.size(); j++) {
                pRtfVerticalColumn = vcols_[j];

                if (pRtfVerticalColumn->type() == FT_TEXT && pRtfVerticalColumn->m_rectReal.left
                        >= Left && pRtfVerticalColumn->m_rectReal.right <= Right) {
                    pGroup->push_back(j); //~слияние секторов в одну колонку по вертикали
                }
            }
        }
    }

    if (hist_spaces_.size()) {
        type_ = FRAME_AND_COLUMN;
        return;
    }

    //~неудача деления => all columns become frames
    for (i = 0; i < vcols_.size(); i++) {
        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_TEXT)
            pRtfVerticalColumn->setType(FT_FRAME);
    }

    int IndexMaxWidthFragment = 0, MaxWidth = 0;

    for (i = 0; i < vcols_.size(); i++) {
        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_FRAME) {
            if ((pRtfVerticalColumn->m_rectReal.right - pRtfVerticalColumn->m_rectReal.left)
                    > MaxWidth) {
                MaxWidth = pRtfVerticalColumn->m_rectReal.right
                        - pRtfVerticalColumn->m_rectReal.left;
                IndexMaxWidthFragment = i;
            }
        }
    }

    pRtfVerticalColumn = vcols_[IndexMaxWidthFragment];
    pRtfVerticalColumn->setType(FT_TEXT);
    terminal_col_group_.push_back(new VectorWord());
    pGroup = terminal_col_group_[0];
    pGroup->push_back(IndexMaxWidthFragment);
    type_ = FRAME_AND_COLUMN;
}

void CRtfHorizontalColumn::fillAllTerminalColumnIndex() {
    terminal_col_idx_.push_back(new VectorWord);
    VectorWord * pGroup = terminal_col_idx_.front();

    short index;

    for (size_t j = 0; j < vcols_.size(); j++) {
        bool FlagChange = false;
        int Top = 320000;
        CRtfVerticalColumn * col;

        for (unsigned short i = 0; i < vcols_.size(); i++) {
            col = vcols_[i];

            if (col->type() == FT_FRAME || col->m_bSortFlag == 1)
                continue;

            if (col->m_rectReal.top < Top) {
                Top = col->m_rectReal.top;
                index = i;
                FlagChange = true;
            }
        }

        if (FlagChange) {
            pGroup->push_back(index);
            col = vcols_[index];
            col->m_bSortFlag = 1;
        }
    }
}

void CRtfHorizontalColumn::fillSingleTerminalColumnIndex() {
    assert(terminal_col_idx_.empty());

    terminal_col_idx_.push_back(new VectorWord);
    terminal_col_idx_.front()->push_back(0);
}

void CRtfHorizontalColumn::fillTerminalFrameColumnIndex() {
    for (size_t j = 0, col_count = terminal_col_group_.size(); j < col_count; j++) {
        terminal_col_idx_.push_back(new VectorWord);
        VectorWord * pGroupNew = terminal_col_idx_[j];
        VectorWord * pGroup = terminal_col_group_[j];

        CRtfVerticalColumn * vcol = NULL;
        int index = 0;

        for (size_t i = 0, CountInGroup = pGroup->size(); i < CountInGroup; i++) {
            bool FlagChange = false;
            int Top = 320000;

            for (size_t i1 = 0; i1 < CountInGroup; i1++) {
                int Number = pGroup->at(i1);
                vcol = vcols_[Number];

                if (vcol->type() == FT_FRAME || vcol->m_bSortFlag == 1)
                    continue;

                if (vcol->m_rectReal.top < Top) {
                    Top = vcol->m_rectReal.top;
                    index = Number;
                    FlagChange = true;
                }
            }

            if (FlagChange) {
                pGroupNew->push_back(index);
                vcol = vcols_[index];
                vcol->m_bSortFlag = 1;
            }
        }
    }
}

void CRtfHorizontalColumn::fillVTerminalColumnsIndex() {
    switch (type_) {
    case SINGLE_TERMINAL:
        fillSingleTerminalColumnIndex();
        break;
        //section includes only terminal columns (! NO frames)
    case ALL_TERMINAL:
        fillAllTerminalColumnIndex();
        break;
    case FRAME_AND_COLUMN:
        fillTerminalFrameColumnIndex();
        break;
    default:
        Debug() << "[CRtfHorizontalColumn::fillVTerminalColumnsIndex] unhandled column type: "
                << type_ << "\n";
        break;
    }
}

int32_t CRtfHorizontalColumn::GetCountAndRightBoundVTerminalColumns(
        VectorWord* arRightBoundTerminalColumns, VectorWord* arWidthTerminalColumns) {
    int32_t CountVTerminalColumns = 0;
    CRtfVerticalColumn *pRtfVerticalColumn;
    uint16_t RightBound, index, WidthColumn, tmp;
    VectorWord *pGroup;

    if (type_ == SINGLE_TERMINAL || type_ == ALL_TERMINAL) {
        RightBound = (uint16_t) MAX(m_rectReal.left, 0);
        WidthColumn = (uint16_t) (m_rectReal.right - m_rectReal.left);
        arRightBoundTerminalColumns->push_back(RightBound);
        arWidthTerminalColumns->push_back(WidthColumn);
        CountVTerminalColumns = 1;
        return CountVTerminalColumns;
    }

    if (type_ == FRAME_AND_COLUMN) {
        CountVTerminalColumns = terminal_col_idx_.size();

        for (int i = 0; i < CountVTerminalColumns; i++) {
            pGroup = terminal_col_idx_[i];
            int CountInGroup = pGroup->size();
            RightBound = 32000;
            WidthColumn = 0;

            for (int j = 0; j < CountInGroup; j++) {
                index = (*pGroup)[j];
                pRtfVerticalColumn = vcols_[index];
                tmp = MAX(pRtfVerticalColumn->m_rectReal.left, 0);
                RightBound = MIN(RightBound, tmp);
                WidthColumn = MAX(WidthColumn, (uint16_t)(pRtfVerticalColumn->m_rectReal.right
                                - pRtfVerticalColumn->m_rectReal.left));
            }

            arRightBoundTerminalColumns->push_back(RightBound);
            arWidthTerminalColumns->push_back(WidthColumn);
        }
    }

    return CountVTerminalColumns;
}

void CRtfHorizontalColumn::WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    CRtfFragment* pRtfFragment;
    int CountFrameInTerminalColumn = vcols_.size();

    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = vcols_[i];
        pRtfFragment = pRtfVerticalColumn->firstFragment();

        if (pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE) {
            if (type_ <= ALL_TERMINAL) {
                pRtfFragment->setInColumn(true);
                SetFlagObjectInColumnForPageFragment(pRtfFragment);
            }

            else {
                SectorInfo->FlagInColumn = FALSE;
                pRtfFragment->setInColumn(false);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            }
        }
    }
}

void CRtfHorizontalColumn::SetFlagObjectInColumnForPageFragment(CRtfFragment* CurFragment) {
    CRtfFragment* pRtfFragment;
    int CountFragments = page_->m_arFragments.size();

    for (int i = 0; i < CountFragments; i++) {
        pRtfFragment = page_->m_arFragments[i];

        if (pRtfFragment->type() == CurFragment->type() && pRtfFragment->m_rect.left
                == CurFragment->m_rect.left && pRtfFragment->m_rect.right
                == CurFragment->m_rect.right && pRtfFragment->m_rect.top == CurFragment->m_rect.top
                && pRtfFragment->m_rect.bottom == CurFragment->m_rect.bottom)
            pRtfFragment->setInColumn(true);
    }
}

void CRtfHorizontalColumn::WriteTerminalColumns(VectorWord* arRightBoundTerminalColumns,
        int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns, RtfSectorInfo *SectorInfo) {
    int colsr(0), i(0), j(0), colw(0), CountInGroup(0);
    int32_t CountTerminalColumns, NextColumnsLeft, CountFrameInTerminalColumn, Left, Right;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    VectorWord *pGroup;
    uint16_t FreeSpace;
    int number;
    Bool FlagFirstTerminalFragment = FALSE;
    int32_t TopPositionFirstTerminalFragment;
    RECT Rect;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************        Tерминальная колонка из одного или нескольких фрагментов  ******************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (type_ == SINGLE_TERMINAL || type_ == ALL_TERMINAL) {
        *VTerminalColumnNumber += 1;

        if (RfrmtOptions::useFramesAndColumns() && *VTerminalColumnNumber == 1
                && CountVTerminalColumns > 1) {
            Rect.top = m_rectReal.top;
            Rect.bottom = m_rectReal.bottom;
            Rect.left = m_rectReal.right;
            Rect.right = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];

            if (CheckLines(&Rect, TRUE, SectorInfo)) {
                CED_SetSectLineBetCol(SectorInfo->hEDSector, TRUE);
            }
        }

        if (RfrmtOptions::useFramesAndColumns() && SectorInfo->FlagOneString == TRUE)
            colw = MAX(0, SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR));
        else
            colw = m_rectReal.right - m_rectReal.left;

        colsr = -1;

        if (CountVTerminalColumns && *VTerminalColumnNumber < CountVTerminalColumns) {
            NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
            colsr = NextColumnsLeft - m_rectReal.right;
        }

        if (*VTerminalColumnNumber == 1)
            SectorInfo->hColumn = SectorInfo->hFirstColumn;
        else
            SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

        SectorInfo->hObject = SectorInfo->hColumn;

        CountFrameInTerminalColumn = vcols_.size();
        SortFragments();

        for (i = 0; i < CountFrameInTerminalColumn; i++) {
            number = i;

            if (m_arOrderingNumber.size())
                number = m_arOrderingNumber[i];

            pRtfVerticalColumn = vcols_[number];
            pRtfFragment = pRtfVerticalColumn->firstFragment();
            FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(pRtfFragment->m_rect.top,
                    SectorInfo);
            SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

            if (pRtfFragment->type() != FT_TABLE && pRtfFragment->type() != FT_PICTURE) {//Text
                pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn = pRtfFragment->m_rect.left
                        - m_rect.left;
                pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right
                        - pRtfFragment->m_rect.right;
                pRtfFragment->m_WidthVerticalColumn = int16_t(m_rect.right - m_rect.left);
                SectorInfo->hObject = SectorInfo->hColumn;
                SectorInfo->FlagOverLayed = GetOverLayedFlag(i);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            } else {
                if ((pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        && pRtfFragment->inColumn()) {//Picture,Table
                    SectorInfo->FlagInColumn = TRUE;

                    if (!SectorInfo->FlagOneString)//!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left - m_rect.left;
                    else
                        //!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left
                                - SectorInfo->MargL;//!!!Art

                    SectorInfo->OffsetFromColumn.ry() = pRtfFragment->m_wOffsetFromPrevTextFragment;
                    SectorInfo->hObject = SectorInfo->hColumn;
                    pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************************                    Фреймы и колонки            ************************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        CountTerminalColumns = terminal_col_idx_.size();

        for (i = 0; i < CountTerminalColumns; i++) {
            *VTerminalColumnNumber += 1;
            Left = 32000;
            Right = 0;
            pGroup = terminal_col_idx_[i];
            CountInGroup = pGroup->size();

            for (j = 0; j < CountInGroup; j++) { //~ V-columns in one H-col
                int index = (*pGroup)[j];
                pRtfVerticalColumn = (CRtfVerticalColumn*) vcols_[index];

                if (i == 0 && j == 0) {
                    FlagFirstTerminalFragment = TRUE;
                    TopPositionFirstTerminalFragment = pRtfVerticalColumn->m_rectReal.top;
                }

                Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left);
                Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
            }

            colw = Right - Left;
            colsr = -1;

            if (*VTerminalColumnNumber < CountVTerminalColumns) {
                NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
                colsr = NextColumnsLeft - Right;
            }

            if (*VTerminalColumnNumber == 1)
                SectorInfo->hColumn = SectorInfo->hFirstColumn;

            else
                SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

            SectorInfo->hObject = SectorInfo->hColumn;

            if (!i) //noisy fragment or picture are made as frame,frames привязаны к первой терминал. колонке сектора
                WriteFramesInTerminalColumn(SectorInfo, FlagFirstTerminalFragment,
                        TopPositionFirstTerminalFragment);

            for (j = 0; j < CountInGroup; j++) {
                int index = (*pGroup)[j];
                pRtfVerticalColumn = (CRtfVerticalColumn*) vcols_[index];
                FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(
                        pRtfVerticalColumn->m_rect.top, SectorInfo);
                SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;
                SectorInfo->hObject = SectorInfo->hColumn;
                SectorInfo->FlagOverLayed = GetOverLayedFlag(index);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            }
        }
    }
}

uint16_t CRtfHorizontalColumn::GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFragment,
        RtfSectorInfo *SectorInfo) {
    CRtfFragment *pRtfFragment;
    RECT FreePlace;
    int i, CountFragments;
    int32_t FreePlaceHeight;
    FreePlace.left = m_rect.left;
    FreePlace.right = m_rect.right;
    FreePlace.bottom = TopPosCurFragment - 1;
    FreePlace.top = MAX(0, m_rect.top - SectorInfo->VerticalOffsetColumnFromSector);

    if (FreePlace.top >= FreePlace.bottom)
        return 0;

    CountFragments = page_->m_arFragments.size();

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = page_->m_arFragments[i];

        if (pRtfFragment->m_rect.bottom <= FreePlace.top || pRtfFragment->m_rect.right
                <= FreePlace.left || pRtfFragment->m_rect.top >= FreePlace.bottom
                || pRtfFragment->m_rect.left >= FreePlace.right)
            continue;

        if (pRtfFragment->m_rect.bottom >= FreePlace.top && pRtfFragment->m_rect.bottom
                <= FreePlace.bottom)
            FreePlace.top = pRtfFragment->m_rect.bottom;
    }

    FreePlaceHeight = MAX(0, FreePlace.bottom - FreePlace.top);
    return (uint16_t) FreePlaceHeight;
}

Bool CRtfHorizontalColumn::GetOverLayedFlag(int CurFragmentNumber) {
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    CIF::Rect CurFragmentRect;
    int i, number, CountFragments;
    CIF::Point pt;
    number = CurFragmentNumber;

    if (m_arOrderingNumber.size())
        number = m_arOrderingNumber[CurFragmentNumber];

    pRtfVerticalColumn = vcols_[number];
    pRtfFragment = pRtfVerticalColumn->firstFragment();
    CurFragmentRect.rleft() = pRtfFragment->m_rect.left;
    CurFragmentRect.rright() = pRtfFragment->m_rect.right;
    CurFragmentRect.rbottom() = pRtfFragment->m_rect.bottom;
    CurFragmentRect.rtop() = pRtfFragment->m_rect.top;
    CountFragments = page_->m_arFragments.size();

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = page_->m_arFragments[i];

        if (!pRtfFragment->type())
            continue;

        pt.rx() = pRtfFragment->m_rect.left;
        pt.ry() = pRtfFragment->m_rect.top;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = pRtfFragment->m_rect.right;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = pRtfFragment->m_rect.left;
        pt.ry() = pRtfFragment->m_rect.bottom;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = pRtfFragment->m_rect.right;

        if (CurFragmentRect.contains(pt))
            return TRUE;
    }

    return FALSE;
}

void CRtfHorizontalColumn::SortFragments() {
    int32_t CountFrameInTerminalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment, *pRtfFragmentFirst;
    int /*NextTop=-32000,*/size, number;
    Bool FlagInserted = FALSE;
    CountFrameInTerminalColumn = vcols_.size();

    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = vcols_[i];
        pRtfFragment = pRtfVerticalColumn->firstFragment();
        FlagInserted = FALSE;

        if (i == 0)
            m_arOrderingNumber.push_back(i);

        else {
            size = m_arOrderingNumber.size();
            int m;

            for (m = 0; m < size; m++) {
                number = m_arOrderingNumber[m];
                pRtfVerticalColumn = vcols_[number];
                pRtfFragmentFirst = pRtfVerticalColumn->firstFragment();

                //Если фрагмент выше другого
                if (pRtfFragment->m_rect.top < pRtfFragmentFirst->m_rect.top) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);

                    if (pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        pRtfFragment->m_wOffsetFromPrevTextFragment
                                = GetOffsetFromPrevTextFragment(pRtfFragment);

                    FlagInserted = TRUE;
                    break;
                }

                else

                //Если таблица/картинка покрывается текстовым фрагментом
                if ((pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        && pRtfFragmentFirst->type() == FT_TEXT && pRtfFragment->m_rect.top
                        >= pRtfFragmentFirst->m_rect.top && pRtfFragment->m_rect.top
                        < pRtfFragmentFirst->m_rect.bottom) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);
                    pRtfFragment->m_wOffsetFromPrevTextFragment
                            = (uint16_t) (pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.top);
                    FlagInserted = TRUE;
                    break;
                }
            }

            if (FlagInserted == FALSE) {
                m_arOrderingNumber.push_back(m);
            }
        }
    }

    //Надо оттестировать
    /*  //Выставляем расстояние для картинок и таблиц, которые пойдут
     //во фреймы после последнго текстового фрагмента
     for( int m=m_arOrderingNumber.size()-1; m>=0; m-- )
     {
     pRtfVerticalColumn = vcols_[m_arOrderingNumber[m]];
     pRtfFragmentFirst   = pRtfVerticalColumn->m_arFragments[0];
     if (pRtfFragmentFirst->type_==FT_TEXT)
     break;
     }
     if (m<0)
     return;
     //нашли последний текстовый фрагмент - выставляем до него расстояние
     int lastTxtNum=m;
     //Ищем высоту фиктивного абзаца+разрыва колонки/раздела
     int16_t parHeight=0,brkHeight=0;
     GetRealSize("A",0,6,0,&parHeight);
     GetRealSize("A",0,24,0,&brkHeight);
     parHeight/=Twips;
     brkHeight/=Twips;
     for( m=m_arOrderingNumber.size()-1; m>lastTxtNum; m-- )
     {
     pRtfVerticalColumn = vcols_[m_arOrderingNumber[m]];
     pRtfFragment   = pRtfVerticalColumn->m_arFragments[0];
     if( (pRtfFragment->type_==FT_TABLE || pRtfFragment->type_==FT_PICTURE))
     {
     //вычитаем 2 высоты - фикт. абзаца и разрыва
     //если меньше, чем высота разрава, то чтобы он не упрыгнул вниз
     pRtfFragment->m_wOffsetFromPrevTextFragment = (uint16_t)MAX(brkHeight,pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.bottom-brkHeight-parHeight);
     }
     }
     */
}

uint16_t CRtfHorizontalColumn::GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment) {
    int32_t CountFrameInTerminalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragmentNext;
    uint16_t VerOffset = 0;
    CountFrameInTerminalColumn = vcols_.size();

    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = vcols_[i];
        pRtfFragmentNext = pRtfVerticalColumn->firstFragment();

        if (pRtfFragmentNext->type() == FT_TEXT && pRtfFragment->m_rect.top
                >= pRtfFragmentNext->m_rect.top && pRtfFragment->m_rect.top
                < pRtfFragmentNext->m_rect.bottom)
            VerOffset = (uint16_t) (pRtfFragment->m_rect.top - pRtfFragmentNext->m_rect.top);
    }

    return VerOffset;
}

void CRtfHorizontalColumn::WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo,
        Bool FlagFirstTerminalFragment, int32_t TopPositionFirstTerminalFragment) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    int32_t shpleft, shptop, shpright, shpbottom, fri = 0;
#ifdef EdWrite
    EDBOX EdFragmRect;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    int align;
    CIF::Rect indent;
    EDSIZE interval;
    EDBOX playout;
#endif

    for (size_t i = 0; i < vcols_.size(); i++) {
        if (!fri) {
            fri = 1;
            indent = CIF::Rect();
            interval.cx = 0;
            interval.cy = 0;
            playout.x = -1;
            playout.w = -1;
            playout.y = -1;
            playout.h = -1;
            align = TP_LEFT_ALLIGN;
            hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align,
                    indent, SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
            hString = CED_CreateLine(hParagraph, FALSE, 6);
        }

        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_FRAME) {
            shpleft = pRtfVerticalColumn->m_rectReal.left - m_rectReal.left;
            shptop = pRtfVerticalColumn->m_rectReal.top - m_rectReal.top;
            shpbottom = pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top;
            shpright = pRtfVerticalColumn->m_rectReal.right - pRtfVerticalColumn->m_rectReal.left;

            EdFragmRect.x = pRtfVerticalColumn->m_rectReal.left - m_rectReal.left;
            EdFragmRect.w = pRtfVerticalColumn->m_rectReal.right
                    - pRtfVerticalColumn->m_rectReal.left;
            EdFragmRect.y = pRtfVerticalColumn->m_rectReal.top - m_rectReal.top;
            EdFragmRect.h = pRtfVerticalColumn->m_rectReal.bottom
                    - pRtfVerticalColumn->m_rectReal.top;
            SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
                    EdFragmRect, 0x22, -1, 86, 43);

            SectorInfo->FlagOverLayed = FALSE;
            pRtfVerticalColumn->write(SectorInfo, FOT_FRAME);
        }
    }
}

void CRtfHorizontalColumn::WriteNonTerminalColumns(RtfSectorInfo* SectorInfo) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn* vcol = vcols_[i];

        if (vcol->type() > FT_FRAME) {
            SectorInfo->FlagInColumn = FALSE;
            vcol->write(SectorInfo, FOT_FRAME);
        }
    }
}

void CRtfHorizontalColumn::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    CRtfFragment* pRtfFragmentNew;
    vcols_.push_back(new CRtfVerticalColumn());

    if (vcols_.size() == 1)
        type_ = ONLY_PICTURE_TABLE;

    pRtfVerticalColumn = vcols_.back();
    pRtfVerticalColumn->setType(pRtfFragment->type());
    pRtfFragmentNew = new CRtfFragment;
    pRtfVerticalColumn->addFragment(pRtfFragmentNew);
    pRtfFragmentNew->setType(pRtfFragment->type());
    pRtfFragmentNew->m_wUserNumber = pRtfFragment->m_wUserNumber;
    pRtfFragmentNew->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumberForFormattedMode;
    RtfUnionRect_CRect_CRect(&pRtfFragmentNew->m_rect, &pRtfFragment->m_rect);
}

}

