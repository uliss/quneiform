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

#include "creatertf.h"
#include "cfcompat.h"

namespace CIF
{

class CRtfPage;
class CRtfHorizontalColumn;

class CRtfSector
{
    public:
        CRtfSector();
        ~CRtfSector();

        CRtfPage *m_PagePtr;

        Bool Write(void);
        void WriteTerminalColumns(void);
        void WriteNonTerminalColumns(void);
        void WriteTerminalColumnsTablesAndPictures(void);
        void CalcSector(void);
        int32_t GetCountAndRightBoundVTerminalColumns(void);
        void ToPlacePicturesAndTables(CRtfFragment* Frament);
        void FillingSectorInfo(void);

        std::vector<CRtfHorizontalColumn*> m_arHorizontalColumns;
        VectorWord m_arHTerminalColumnsIndex;
        VectorWord m_arRightBoundTerminalColumns;
        VectorWord m_arWidthTerminalColumns;
        RtfSectorInfo SectorInfo;

        uint16_t m_wHorizontalColumnsCount;
        uint16_t m_wFragmentsCount;
        uint16_t m_wTextFragmentsCount;
        uint16_t m_wPictureFragmentsCount;
        uint16_t m_wTableFragmentsCount;
        uint16_t m_wIndex;
        RECT m_rect;
        RECT m_rectReal;
        uint16_t m_wType;
        Handle m_hEDSector;
        Bool m_FlagOneString;
        Bool m_bFlagLine;
        int32_t m_VTerminalColumnNumber;
};

}

#endif /* CRTFSECTOR_H_ */
