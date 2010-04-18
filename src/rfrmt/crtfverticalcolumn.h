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

#ifndef CRTFVERTICALCOLUMN_H_
#define CRTFVERTICALCOLUMN_H_

#include <vector>
#include "cfcompat.h"

class RtfSectorInfo;

namespace CIF
{

class CRtfPage;
class CRtfFragment;

class CRtfVerticalColumn
{
    public:
        CRtfVerticalColumn();
        ~CRtfVerticalColumn();

        Bool Write(Bool OutPutType, RtfSectorInfo* SectorInfo);
        void SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo);

        std::vector<CRtfFragment*> m_arFragments;
        CRtfPage *m_PagePtr;

        uint16_t m_wFragmentsCount;
        uint16_t m_wIndex;
        Bool m_bSortFlag;
        RECT m_rect;
        RECT m_rectReal;
        uint16_t m_wType;
        Bool m_bFlagObjectInColumn;
        Bool m_bFlagSmall;
};

}

#endif /* CRTFVERTICALCOLUMN_H_ */
