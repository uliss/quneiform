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

#ifndef CRTFSECTOR_H_
#define CRTFSECTOR_H_

#include <vector>
#include <boost/function.hpp>

#include "creatertf.h"
#include "cfcompat.h"

namespace CIF
{

class CRtfPage;
class CRtfHorizontalColumn;
class CRtfSector;

typedef boost::function<void(const CRtfSector*)> RfrmtDrawSectorFunction;

class CRtfSector
{
    public:
        CRtfSector();
        ~CRtfSector();

        /**
         * Adds horizontal column to sector
         */
        void addColumn(CRtfHorizontalColumn * col);

        void calcSector();

        /**
         * Removes all column from sector
         */
        void clearColumns();

        /**
         * Returns number of columns in sector
         */
        size_t columnCount() const;

        /**
         * Sets pointer to CRtfPage parent
         */
        void setPage(CRtfPage * page);

        Bool Write();
        void WriteTerminalColumns();
        void WriteNonTerminalColumns();
        void WriteTerminalColumnsTablesAndPictures();
        int32_t GetCountAndRightBoundVTerminalColumns();
        void ToPlacePicturesAndTables(CRtfFragment* Frament);
        void FillingSectorInfo();

        VectorWord m_arRightBoundTerminalColumns;
        VectorWord m_arWidthTerminalColumns;
        RtfSectorInfo SectorInfo;

        RECT m_rect;
        RECT m_rectReal;
        Handle m_hEDSector;
        Bool m_FlagOneString;
        Bool m_bFlagLine;
        int m_VTerminalColumnNumber;
    public:
        static void setDrawCallback(RfrmtDrawSectorFunction f);
    private:
        static RfrmtDrawSectorFunction draw_func_;
    private:
        typedef std::vector<CRtfHorizontalColumn*> HColumnList;
        typedef HColumnList::iterator iterator;
    private:
        CRtfPage * page_;
        HColumnList hcols_;
        VectorWord terminal_col_idx_;

};

}

#endif /* CRTFSECTOR_H_ */
