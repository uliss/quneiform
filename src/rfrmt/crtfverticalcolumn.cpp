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

#include "crtfverticalcolumn.h"
#include "creatertf.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "minmax.h"

namespace CIF
{

CRtfVerticalColumn::CRtfVerticalColumn() :
    page_(NULL), type_(FT_TEXT), small_(false), sorted_(false) {
    SetRect(&m_rect, 32000, 32000, 0, 0);
}

CRtfVerticalColumn::~CRtfVerticalColumn() {
    clearFragments();
}

void CRtfVerticalColumn::addFragment(CRtfFragment * fragment) {
    fragments_.push_back(fragment);
}

void CRtfVerticalColumn::clearFragments() {
    for (FragmentIterator it = fragments_.begin(), end = fragments_.end(); it != end; ++it) {
        delete (*it);
    }

    fragments_.clear();
}

CRtfFragment * CRtfVerticalColumn::firstFragment() {
    return fragments_.at(0);
}

const CRtfFragment * CRtfVerticalColumn::firstFragment() const {
    return fragments_.at(0);
}

CRtfFragment * CRtfVerticalColumn::fragmentAt(size_t pos) {
    return fragments_.at(pos);
}

const CRtfFragment * CRtfVerticalColumn::fragmentAt(size_t pos) const {
    return fragments_.at(pos);
}

size_t CRtfVerticalColumn::fragmentCount() const {
    return fragments_.size();
}

bool CRtfVerticalColumn::isSmall() const {
    return small_;
}

bool CRtfVerticalColumn::isSorted() const {
    return sorted_;
}

CRtfPage * CRtfVerticalColumn::page() {
    return page_;
}

const CRtfPage * CRtfVerticalColumn::page() const {
    return page_;
}

int CRtfVerticalColumn::realHeight() const {
    return m_rectReal.bottom - m_rectReal.top;
}

int CRtfVerticalColumn::realWidth() const {
    return m_rectReal.right - m_rectReal.left;
}

void CRtfVerticalColumn::setPage(CRtfPage * page) {
    page_ = page;
}

void CRtfVerticalColumn::setRealRect(const Rect& r) {
    m_rectReal.top = r.top();
    m_rectReal.left = r.left();
    m_rectReal.bottom = r.bottom();
    m_rectReal.right = r.right();
}

void CRtfVerticalColumn::setSmall(bool value) {
    small_ = value;
}

void CRtfVerticalColumn::setSorted(bool value) {
    sorted_ = value;
}

void CRtfVerticalColumn::setType(fragment_t type) {
    type_ = type;
}

fragment_t CRtfVerticalColumn::type() const {
    return type_;
}

void CRtfVerticalColumn::write(RtfSectorInfo * sector, fragment_output_t type) {
    for (FragmentIterator it = fragments_.begin(), end = fragments_.end(); it != end; ++it) {
        CRtfFragment * fragm = *it;
        if (!fragm->m_LeftOffsetFragmentFromVerticalColumn
                && !fragm->m_RightOffsetFragmentFromVerticalColumn) {
            fragm->m_LeftOffsetFragmentFromVerticalColumn = m_rect.left - fragm->m_rect.left;
            fragm->m_RightOffsetFragmentFromVerticalColumn = m_rect.right - fragm->m_rect.right;
        }

        if (!fragm->m_WidthVerticalColumn)
            fragm->m_WidthVerticalColumn = m_rect.right - m_rect.left;

        fragm->setParent(page_);
        fragm->write(sector, type);
    }
}

void CRtfVerticalColumn::writeTablesAndPictures(RtfSectorInfo * SectorInfo, bool allTerminal) {
    if (fragments_.empty())
        return;

    CRtfFragment * frag = firstFragment();

    if (frag->type() == FT_TABLE || frag->type() == FT_PICTURE) {
        if (allTerminal) {
            frag->setInColumn(true);
            page_->setFragmentsInColumn(frag);
        } else {
            SectorInfo->FlagInColumn = FALSE;
            frag->setInColumn(false);
            write(SectorInfo, FOT_SINGLE);
        }
    }
}

}

