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

namespace CIF
{

CEDChar::CEDChar() :
    parent_number_(0), font_height_(0), font_lang_(LANGUAGE_ENGLISH) {
    fontNum = font_style_ = fontNum = 0;
    alternatives = 0;
    numOfAltern = 0;
    prev = next = 0;
}

Color CEDChar::backgroundColor() const {
    return bground_color_;
}

Color CEDChar::foregroundColor() const {
    return fground_color_;
}

bool CEDChar::isPicture() const {
    return (fontNum >= ED_PICT_BASE) && (fontNum != 0xffffffff) && (fontNum != 0xfffffffe);
}

Rect CEDChar::boundingRect() const {
    return bbox_;
}

int CEDChar::fontHeight() const {
    return font_height_;
}

language_t CEDChar::fontLanguage() const {
    return font_lang_;
}

int CEDChar::fontStyle() const {
    return font_style_;
}

int CEDChar::parentNumber() const {
    return parent_number_;
}

Rect& CEDChar::rBoundingRect() {
    return bbox_;
}

void CEDChar::setBackgroundColor(const Color& color) {
    bground_color_ = color;
}

void CEDChar::setBoundingRect(const Rect& bbox) {
    bbox_ = bbox;
}

void CEDChar::setBoundingRect(const EDBOX& bbox) {
    bbox_.rleft() = bbox.x;
    bbox_.rtop() = bbox.y;
    bbox_.setWidth(bbox.w);
    bbox_.setHeight(bbox.h);
}

void CEDChar::setFontHeight(int height) {
    font_height_ = height;
}

void CEDChar::setFontLanguage(language_t lang) {
    font_lang_ = lang;
}

void CEDChar::setFontStyle(int style) {
    font_style_ = style;
}

void CEDChar::setForegroundColor(const Color& color) {
    fground_color_ = color;
}

void CEDChar::setParentNumber(int number) {
    parent_number_ = number;
}

std::ostream& operator<<(std::ostream& os, const CEDChar& chr) {
    os << "char: parentNumber=" << chr.parentNumber() << ";fontheight=" << chr.fontHeight();
    return os;
}

}
