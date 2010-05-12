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
#include "creatertf.h"

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

        /**
         * Adds fragment to the end of column
         */
        void addFragment(CRtfFragment * fragment);

        /**
         * Removes all fragments from column
         */
        void clearFragments();

        /**
         * Returns pointer to first fragment in column
         * @throw std::out_of_range if column is empty
         */
        CRtfFragment * firstFragment();
        const CRtfFragment * firstFragment() const;

        /**
         * Returns pointer to fragment
         * @param pos - position of fragment
         * @throw std::out_of_range if fragment not exists at such position
         */
        CRtfFragment * fragmentAt(size_t pos);
        const CRtfFragment * fragmentAt(size_t pos) const;

        /**
         * Returns fragment count
         */
        size_t fragmentCount() const;

        /**
         * Returns pointer to parent page
         * @return
         */
        CRtfPage * page();
        const CRtfPage * page() const;

        /**
         * Sets pointer to parent page
         * @param page
         */
        void setPage(CRtfPage * page);

        /**
         * Sets column type
         */
        void setType(fragment_t type);

        /**
         * Returns column type
         */
        fragment_t type() const;

        /**
         * Exports column to CED document structure
         * @param type
         * @param SectorInfo
         */
        void write(RtfSectorInfo * SectorInfo, fragment_output_t type);

        Bool m_bSortFlag;
        RECT m_rect;
        RECT m_rectReal;
        Bool m_bFlagSmall;
    private:
        typedef std::vector<CRtfFragment*> FragmentList;
        typedef FragmentList::iterator FragmentIterator;
        FragmentList fragments_;
        CRtfPage * page_;
        fragment_t m_wType;
};

}

#endif /* CRTFVERTICALCOLUMN_H_ */
