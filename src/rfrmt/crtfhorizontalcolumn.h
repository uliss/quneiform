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

#ifndef CRTFHORIZONTALCOLUMN_H_
#define CRTFHORIZONTALCOLUMN_H_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "cfcompat.h"
#include "common/histogram.h"
#include "creatertf.h"

namespace CIF
{

class CRtfPage;
class CRtfVerticalColumn;
class CRtfFragment;

class CLA_EXPO CRtfHorizontalColumn
{
    public:
        enum column_t
        {
            SINGLE_TERMINAL = 0,
            ALL_TERMINAL = 1,
            FRAME_AND_COLUMN = 2,
            ALL_FRAME = 3,
            ONLY_PICTURE_TABLE = 4
        };
    public:
        CRtfHorizontalColumn();
        ~CRtfHorizontalColumn();

        /**
         * Adds vertical column
         */
        void addColumn(CRtfVerticalColumn * col);

        /**
         * Try to divide into columns
         */
        void calcHorizontalColumn();

        /**
         * Removes all vertical columns
         */
        void clearColumns();

        /**
         * Returns pointer to vertical column at position @b pos
         * @throw std::out_of_range if invalid position given
         */
        CRtfVerticalColumn * columnAt(size_t pos);
        const CRtfVerticalColumn * columnAt(size_t pos) const;

        /**
         * Returns number of vertical columns
         */
        size_t columnCount() const;

        /**
         * Sets pointer to parent page
         */
        void setPage(CRtfPage * page);

        /**
         * Sets column type
         */
        void setType(column_t type);

        /**
         * Returns column type
         */
        column_t type() const;

        void writeNonTerminalColumns(RtfSectorInfo* SectorInfo);

        Bool Write(VectorWord* arRightBoundTerminalColumns, int32_t* VTerminalColumnNumber);
        void WriteTerminalColumns(VectorWord* arRightBoundTerminalColumns,
                int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns,
                RtfSectorInfo* SectorInfo);
        void WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
        int32_t GetCountAndRightBoundVTerminalColumns(VectorWord* arRightBoundTerminalColumns,
                VectorWord* arWidthTerminalColumns);
        void ToPlacePicturesAndTables(CRtfFragment* Frament);

        RECT m_rect;
        RECT m_rectReal;
    private:
        /* sets all text vertical columns to frames */
        void allTextToFrames();
        bool checkTerminalColumn() const;
        /* recalculation of histogram after victim deletion */
        void defineTerminalProperty();
        /*
         * Called if division to columns failed. All column become frames. (FT_FRAME)
         * Then widest frame selected and marked as FT_TEXT
         */
        void divisionFailed();

        /*
         * resorting of the fragment array (at first position should be located Frames,
         * next ones are terminal columns)
         */
        void fillVTerminalColumnsIndex();
        void fillAllTerminalColumnIndex();
        void fillSingleTerminalColumnIndex();
        void fillTerminalFrameColumnIndex();
        void fillTerminalGroups(int minLeft, int maxRight);
        void findHeadingAndSetFrameFlag();
        uint16_t GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag,
                RtfSectorInfo *SectorInfo);
        uint16_t GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment);
        Bool GetOverLayedFlag(int CurFragmentNumber);
        /* detects and marks small vertical columns */
        void markSmallColumns();
        int maxVColumnHeight() const;
        int maxVColumnWidth() const;
        void processColsByHist(const Histogram& hist, int left_offset);
        void processSpaceByHist(const Histogram& hist);
        void setFlagObjectInColumnForPageFragment(CRtfFragment* CurFragment);
        void sortFragments();
        void writeFramesInTerminalColumn(RtfSectorInfo* SectorInfo, Bool FlagFirstTerminalFragment);
    private:
        typedef std::vector<CRtfVerticalColumn*> VColumnList;
        typedef VColumnList::iterator VColumnIterator;
        typedef VColumnList::const_iterator VColumnIteratorConst;
        typedef std::vector<int> IndexList;
        typedef boost::shared_ptr<IndexList> IndexListPtr;
        typedef std::vector<IndexListPtr> TerminalIndexes;
        VColumnList vcols_;
        CRtfPage * page_;
        column_t type_;
        std::vector<uchar> m_arOrderingNumber;
        // contains histogram positions where spaces starts
        // for ex. for histogram 0011001100 - contains (0,4,8)
        IndexList hist_spaces_;
        TerminalIndexes terminal_col_group_;
        TerminalIndexes terminal_col_idx_;
};

}

#endif /* CRTFHORIZONTALCOLUMN_H_ */
