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
        void writeTerminalColumns(VectorWord* arRightBoundTerminalColumns,
                int * VTerminalColumnNumber, int CountVTerminalColumns, RtfSectorInfo* SectorInfo);
        void writeTablesAndPictures(RtfSectorInfo * sector);

        int getCountAndRightBoundVTerminalColumns(VectorWord * RightBoundTerminalColumns,
                VectorWord * WidthTerminalColumns);
        void ToPlacePicturesAndTables(CRtfFragment* Frament);

        RECT m_rect;
        RECT m_rectReal;
    private:
        typedef std::vector<CRtfVerticalColumn*> VColumnList;
        typedef VColumnList::iterator VColumnIterator;
        typedef VColumnList::const_iterator VColumnIteratorConst;
        typedef std::vector<int> IndexList;
        typedef boost::shared_ptr<IndexList> IndexListPtr;
        typedef std::vector<IndexListPtr> TerminalIndexes;
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

        void fillAllTerminalColumnIndex();
        void fillSingleTerminalColumnIndex();

        /*
         * resorting of the fragment array (at first position should be located Frames,
         * next ones are terminal columns)
         */
        void fillTerminalColumnsIndex();
        void fillTerminalFrameColumnIndex();
        void fillTerminalGroups(int minLeft, int maxRight);
        void findHeadingAndSetFrameFlag();

        /*
         * return index of highest unsorted column (skipping frames)
         * or -1 if nothing found
         */
        int findHighestUnsortedColumn() const;

        /*
         * return index of highest unsorted column (skipping frames) in given group
         * or -1 if nothing found
         * @throw std::out_of_range if column index in group is more then size of vcols_
         */
        int findHighestUnsortedColumnInGroup(const IndexList& group) const;
        uint16_t GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag,
                RtfSectorInfo *SectorInfo);
        int getOffsetFromPrevTextFragment(const CRtfFragment *pRtfFragment);
        Bool getOverLayedFlag(int CurFragmentNumber);
        /* detects and marks small vertical columns */
        void markSmallColumns();
        int maxVColumnHeight() const;
        int maxVColumnWidth() const;
        /*
         * merges vertical columns that lays inside (minLeft <= col <= maxRight) borders
         * into one column group
         */
        void mergeColumnsToGroup(int minLeft, int maxRight, IndexList * group);
        void processColsByHist(const Histogram& hist, int left_offset);
        void processSpaceByHist(const Histogram& hist);
        void sortFragments();

        /*
         * sorts columns by top position - higher columns are earlier
         * columns in @b vcols_ are marked as sorted
         * @param dest_idx - write indexes of sorted columns to given array
         * @note dest_idx is not cleared
         */
        void sortColumns(IndexList * dest_idx);
        void sortColumnsInGroup(const IndexList& group, IndexList * dest_idx);
        void writeFramesInTerminalColumn(RtfSectorInfo* SectorInfo);
        void writeTerminalColumnsOnly(VectorWord* arRightBoundTerminalColumns,
                int * VTerminalColumnNumber, int CountVTerminalColumns, RtfSectorInfo *SectorInfo);
    private:
        VColumnList vcols_;
        CRtfPage * page_;
        column_t type_;
        std::vector<uchar> ordering_number_;
        // contains histogram positions where spaces starts
        // for ex. for histogram 0011001100 - contains (0,4,8)
        IndexList hist_spaces_;
        // contains indexes of vertical columns in 2-dimension array
        // for example: {{0,1,2} {3,4}, {5}} means that there's 3 column groups
        // in first - vertical columns with indexes == 0,1,2 are merged vertically
        // in second - vertical column with indexes == 3,4 are merged vertically
        // in third - single column with index == 5
        TerminalIndexes terminal_col_group_;
        TerminalIndexes terminal_col_idx_;
};

}

#endif /* CRTFHORIZONTALCOLUMN_H_ */
