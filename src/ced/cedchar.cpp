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

#include "cedchar.h"
#include "cedexporter.h"

namespace CIF
{

CEDChar::CEDChar(Element * parent) :
    Element(parent), font_lang_(LANGUAGE_ENGLISH), font_height_(0), font_style_(0), font_number_(0) {
}

CEDChar::~CEDChar() {
}

void CEDChar::addAlternative(const Letter& letter) {
    alternatives_.push_back(letter);
}

Letter CEDChar::alternativeAt(size_t pos) const {
    return alternatives_.at(pos);
}

size_t CEDChar::alternativeCount() const {
    return alternatives_.size();
}

void CEDChar::exportElement(CEDExporter& exp) {
    exp.exportChar(*this);
}

bool CEDChar::hasAlternatives() const {
    return !alternatives_.empty();
}

int CEDChar::fontHeight() const {
    return font_height_;
}

language_t CEDChar::fontLanguage() const {
    return font_lang_;
}

int CEDChar::fontNumber() const {
    return font_number_;
}

int CEDChar::fontStyle() const {
    return font_style_;
}

void CEDChar::setAlternative(const Letter& letter, size_t pos) {
    alternatives_.at(pos) = letter;
}

void CEDChar::setFontHeight(int height) {
    font_height_ = height;
}

void CEDChar::setFontLanguage(language_t lang) {
    font_lang_ = lang;
}

void CEDChar::setFontNumber(int number) {
    font_number_ = number;
}

void CEDChar::setFontStyle(int style) {
    font_style_ = style;
}

std::ostream& operator<<(std::ostream& os, const CEDChar& chr) {
    os << "char: fontheight=" << chr.fontHeight();
    return os;
}

}
