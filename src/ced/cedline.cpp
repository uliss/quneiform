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

#include "cedline.h"
#include "cedchar.h"

namespace CIF
{

CEDLine::CEDLine() :
    hard_break_(false), internal_number_(0), parent_number_(0),
            default_font_height_(-1) {
}

CEDLine::CEDLine(bool hardBreak, int fontHeight) :
    hard_break_(hardBreak), internal_number_(0), parent_number_(0),
            default_font_height_(fontHeight) {
}

int CEDLine::defaultFontHeight() const {
    return default_font_height_;
}

CEDChar * CEDLine::first() {
    return chars_.at(0).get();
}

bool CEDLine::hardBreak() const {
    return hard_break_;
}

int CEDLine::internalNumber() const {
    return internal_number_;
}

int CEDLine::parentNumber() const {
    return parent_number_;
}

void CEDLine::setDefaultFontHeight(int height) {
    default_font_height_ = height;
}

void CEDLine::setHardBreak(bool value) {
    hard_break_ = value;
}

void CEDLine::setParentNumber(int number) {
    parent_number_ = number;
}

CEDChar * CEDLine::insertChar() {
    return insertChar(new CEDChar);
}

CEDChar * CEDLine::insertChar(CEDChar * chr) {
    assert(chr);
    chr->setParentNumber(internal_number_);
    chars_.push_back(CharPtr(chr));
    return chr;
}

CEDChar * CEDLine::charAt(size_t pos) {
    return chars_.at(pos).get();
}

int CEDLine::charCount() {
    return chars_.size();
}

void CEDLine::clearChars() {
    chars_.clear();
}

}

