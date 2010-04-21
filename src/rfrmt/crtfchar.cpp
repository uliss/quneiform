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

#include <cassert>

#include "crtfchar.h"
#include "recdefs.h"

namespace CIF
{

CRtfChar::CRtfChar() :
    m_bFlg_spell(FALSE), m_bFlg_spell_nocarrying(FALSE), m_bFlg_cup_drop(FALSE), language_(
            LANGUAGE_UNKNOWN), font_number_(0), font_size_(0) {

}

void CRtfChar::addVersion(const Letter& version) {
    if(versions_.size() >= REC_MAX_VERS)
        throw std::out_of_range("[CRtfChar::addVersion] too many char versions");
    versions_.push_back(version);
}

const Letter& CRtfChar::first() const {
    return versions_.front();
}

Letter& CRtfChar::first() {
    return versions_.front();
}

font_number CRtfChar::font() const {
    return font_number_;
}

short CRtfChar::fontSize() const {
    return font_size_;
}

Rect CRtfChar::idealRect() const {
    return ideal_rect_;
}

language_t CRtfChar::language() const {
    return language_;
}

Rect CRtfChar::realRect() const {
    return real_rect_;
}

void CRtfChar::setFont(font_number fontNumber) {
    font_number_ = fontNumber;
}

void CRtfChar::setFontSize(short size) {
    font_size_ = size;
}

void CRtfChar::setIdealRect(const Rect& rect) {
    ideal_rect_ = rect;
}

void CRtfChar::setLanguage(language_t lang) {
    language_ = lang;
}

void CRtfChar::setRealRect(const Rect& rect) {
    real_rect_ = rect;
}

const Letter& CRtfChar::versionAt(size_t pos) const {
    return versions_.at(pos);
}

Letter& CRtfChar::versionAt(size_t pos) {
    return versions_.at(pos);
}

size_t CRtfChar::versionCount() const {
    return versions_.size();
}

}

