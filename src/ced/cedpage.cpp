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

#include <cstdlib>
#include <cstring>
#include "cedpage.h"
#include "cedchar.h"
#include "cedline.h"
#include "cedsection.h"
#include "cedparagraph.h"

namespace CIF
{

CEDPage::CEDPage() :
    turn_(0), page_number_(0), language_(LANGUAGE_RUS_ENG), unrecognized_char_('~') {
}

CEDPage::~CEDPage() {
}

void CEDPage::addFont(const FontEntry& font) {
    fonts_.push_back(font);
}

void CEDPage::addSection(CEDSection * sect) {
    addElement(sect);
}

const FontEntry& CEDPage::fontAt(size_t pos) const {
    return fonts_.at(pos);
}

size_t CEDPage::fontCount() const {
    return fonts_.size();
}

Size CEDPage::imageDpi() const {
    return image_dpi_;
}

std::string CEDPage::imageName() const {
    return image_name_;
}

Size CEDPage::imageSize() const {
    return image_size_;
}

language_t CEDPage::language() const {
    return language_;
}

Rect CEDPage::pageBorder() const {
    return page_borders_;
}

int CEDPage::pageNumber() const {
    return page_number_;
}

Size CEDPage::pageSize() const {
    return page_size_;
}

void CEDPage::setImageDpi(const Size& dpi) {
    image_dpi_ = dpi;
}

void CEDPage::setImageName(const std::string& filename) {
    image_name_ = filename;
}

void CEDPage::setImageSize(const Size& size) {
    image_size_ = size;
}

void CEDPage::setLanguage(language_t language) {
    language_ = language;
}

void CEDPage::setPageBorder(const Rect& borders) {
    page_borders_ = borders;
}

void CEDPage::setPageNumber(int number) {
    page_number_ = number;
}

void CEDPage::setPageSize(const Size& size) {
    page_size_ = size;
}

void CEDPage::setTurn(int angle) {
    turn_ = angle;
}

void CEDPage::setUnrecognizedChar(char chr) {
    unrecognized_char_ = chr;
}

int CEDPage::turn() const {
    return turn_;
}

char CEDPage::unrecognizedChar() const {
    return unrecognized_char_;
}

CEDSection * CEDPage::sectionAt(size_t pos) {
    return dynamic_cast<CEDSection*> (elementAt(pos));
}

size_t CEDPage::sectionCount() const {
    return elementCount();
}

}

