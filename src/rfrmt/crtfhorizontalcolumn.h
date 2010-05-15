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
#include "cfcompat.h"
#include "creatertf.h"

namespace CIF
{

class CRtfPage;
class CRtfVerticalColumn;
class CRtfFragment;

class CRtfHorizontalColumn
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

        Bool Write(VectorWord* arRightBoundTerminalColumns, int32_t* VTerminalColumnNumber);
        void WriteTerminalColumns(VectorWord* arRightBoundTerminalColumns,
                int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns,
                RtfSectorInfo* SectorInfo);
        void WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo, Bool FlagFirstTerminalFragment,
                int32_t TopPositionFirstTerminalFragment);
        void WriteNonTerminalColumns(RtfSectorInfo* SectorInfo);
        void WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
        int32_t GetCountAndRightBoundVTerminalColumns(VectorWord* arRightBoundTerminalColumns,
                VectorWord* arWidthTerminalColumns);
        void FindHeadingAndSetFrameFlag(void);
        void DefineTerminalProperty(void);
        void FillingVTerminalColumnsIndex(void);
        void ToPlacePicturesAndTables(CRtfFragment* Frament);
        void SortFragments(void);
        uint16_t GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag,
                RtfSectorInfo *SectorInfo);
        uint16_t GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment);
        Bool GetOverLayedFlag(int CurFragmentNumber);
        void SetFlagObjectInColumnForPageFragment(CRtfFragment* CurFragment);

        RECT m_rect;
        RECT m_rectReal;
    private:
        bool checkTermColumn() const;
        void clearTerminalColumnsGroup();
        void clearTerminalColumnsIndexes();
    private:
        typedef std::vector<CRtfVerticalColumn*> VColumnList;
        typedef VColumnList::iterator VColumnIterator;
        typedef std::vector<VectorWord*> VectorWordList;
        typedef VectorWordList::iterator VectorWordIterator;
        VColumnList vcols_;
        CRtfPage * page_;
        column_t type_;
        std::vector<uchar> m_arOrderingNumber;
        VectorWord m_arSpacePlace;
        VectorWordList terminal_col_group_;
        VectorWordList terminal_col_idx_;
};

}

#endif /* CRTFHORIZONTALCOLUMN_H_ */
