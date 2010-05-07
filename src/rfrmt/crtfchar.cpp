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
#include <cctype> // for isdigit
#include <iostream>

#include "crtfchar.h"
#include "rfrmtoptions.h"

#include "cstr/cstr.h"
#include "recdefs.h"
#include "edfile.h"

// ced
#include "ced/ced.h"
#include "ced/cedchar.h"

extern CIF::Point TemplateOffset;

namespace CIF
{

CRtfChar::CRtfChar() :
    m_bFlg_spell_nocarrying(FALSE), language_(LANGUAGE_UNKNOWN), font_number_(0), font_size_(0),
            spelled_(false), drop_cap_(false) {

}

CRtfChar::CRtfChar(uchar chr, uchar probability) :
    m_bFlg_spell_nocarrying(FALSE), language_(LANGUAGE_UNKNOWN), font_number_(0), font_size_(0),
            spelled_(false), drop_cap_(false) {
    addVersion(Letter(chr, probability));
}

void CRtfChar::addVersion(const Letter& version) {
    if (versions_.size() >= REC_MAX_VERS)
        throw std::out_of_range("[CRtfChar::addVersion] too many char versions");
    versions_.push_back(version);
}

bool CRtfChar::empty() const {
    return versions_.empty();
}

const Letter& CRtfChar::first() const {
    if (versions_.empty())
        throw std::out_of_range("[CRtfChar::first] no char versions");
    return versions_.front();
}

Letter& CRtfChar::first() {
    return const_cast<Letter&> (const_cast<const CRtfChar*> (this)->first());
}

font_number CRtfChar::font() const {
    return font_number_;
}

short CRtfChar::fontSize() const {
    return font_size_;
}

unsigned char CRtfChar::getChar() const {
    return versions_.empty() ? uchar(0) : versions_.front().getChar();
}

Rect CRtfChar::idealRect() const {
    return ideal_rect_;
}

bool CRtfChar::isDigit() const {
    if (versions_.empty())
        return false;

    return isdigit(versions_.front().getChar());
}

bool CRtfChar::isDropCap() const {
    return drop_cap_;
}

bool CRtfChar::isSpelled() const {
    return spelled_;
}

language_t CRtfChar::language() const {
    return language_;
}

CEDChar * CRtfChar::makeCedSpace(int fontName, int fontSize, int fontAttrs) {
    CEDChar * chr = new CEDChar;

    chr->setFontLanguage(LANGUAGE_UNKNOWN);
    chr->addAlternative(Letter(' ', 0));
    chr->setForegroundColor(Color::null());
    chr->setBackgroundColor(Color::null());
    chr->setBoundingRect(Rect(Point(-1, -1), Point(-1, -1)));

    return chr;
}

Rect CRtfChar::realRect() const {
    return real_rect_;
}

void CRtfChar::setDropCap(bool value) {
    drop_cap_ = value;
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

void CRtfChar::setSpelled(bool value) {
    spelled_ = value;
}

CEDChar * CRtfChar::toCedChar(int font_name, int font_size, int font_style) const {
    CEDChar * chr = new CEDChar;
    Rect layout;
    layout.rleft() = real_rect_.left() + TemplateOffset.x();
    layout.rright() = real_rect_.right() + TemplateOffset.x();
    layout.rtop() = real_rect_.top() + TemplateOffset.y();
    layout.rbottom() = real_rect_.bottom() + TemplateOffset.y();

    chr->setBoundingRect(layout);
    chr->setFontLanguage(language_);
    chr->setFontStyle(font_style);
    chr->setFontNumber(font_name);
    chr->setFontHeight(font_size);

    for (size_t i = 0; i < versions_.size(); i++) {
        Letter alt = versions_[i];
        alt.normalizeNonPrintable();
        chr->addAlternative(alt);
    }

    chr->setForegroundColor(Color::null());
    chr->setBackgroundColor(Color::null());

    return chr;
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

