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
        CRtfHorizontalColumn();
        ~CRtfHorizontalColumn();

        CRtfPage *m_PagePtr;

        Bool Write(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,
                int32_t* VTerminalColumnNumber);
        void WriteTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,
                int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns,
                RtfSectorInfo* SectorInfo);
        void WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo, Bool FlagFirstTerminalFragment,
                int32_t TopPositionFirstTerminalFragment);
        void WriteNonTerminalColumns(RtfSectorInfo* SectorInfo);
        void WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
        void CalcHorizontalColumn(void);
        int32_t GetCountAndRightBoundVTerminalColumns(
        /*CWordArray*/vectorWord* arRightBoundTerminalColumns,
                vectorWord/*CWordArray*/* arWidthTerminalColumns);
        void FindHeadingAndSetFrameFlag(void);
        void DefineTerminalProperty(void);
        void FillingVTerminalColumnsIndex(void);
        void ToPlacePicturesAndTables(CRtfFragment* Frament);
        Bool CheckTermColumn(void);
        void SortFragments(void);
        uint16_t GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag,
                RtfSectorInfo *SectorInfo);
        uint16_t GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment);
        Bool GetOverLayedFlag(int CurFragmentNumber);
        void SetFlagObjectInColumnForPageFragment(CRtfFragment* CurFragment);

        std::vector<CRtfVerticalColumn*> m_arVerticalColumns;
        std::vector<vectorWord*> m_arVTerminalColumnsGroup;
        std::vector<vectorWord*> m_arVTerminalColumnsIndex;
        vectorWord m_arSpacePlace;
        std::vector<uchar> m_arOrderingNumber;

        uint16_t m_wVerticalColumnsCount;
        uint16_t m_wIndex;
        RECT m_rect;
        RECT m_rectReal;
        uint16_t m_wType;
};

}

#endif /* CRTFHORIZONTALCOLUMN_H_ */
