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

#include "blockelement.h"

namespace CIF
{

BlockElement::BlockElement(BlockElement * parent) :
    Element(parent), border_left_(ED_BRDR_NONE), border_top_(ED_BRDR_NONE), border_right_(
            ED_BRDR_NONE), border_bottom_(ED_BRDR_NONE), border_left_wd_(0), border_top_wd_(0),
            border_right_wd_(0), border_bottom_wd_(0) {
}

BlockElement::~BlockElement() {
}

Element * BlockElement::elementAt(size_t pos) {
    return elements_.at(pos).get();
}

void BlockElement::clear() {
    elements_.clear();
}

size_t BlockElement::elementCount() const {
    return elements_.size();
}

bool BlockElement::empty() const {
    return elements_.empty();
}

void BlockElement::addElement(Element * e) {
    addElement(ElementPtr(e));
}

void BlockElement::addElement(ElementPtr e) {
    elements_.push_back(e);
    e->setParent(this);
}

void BlockElement::setBorderBottom(ed_border_t bottom) {
    border_bottom_ = bottom;
}

void BlockElement::setBorderBottomWidth(unsigned short width) {
    border_bottom_wd_ = width;
}

void BlockElement::setBorderLeft(ed_border_t left) {
    border_left_ = left;
}

void BlockElement::setBorderLeftWidth(unsigned short width) {
    border_left_wd_ = width;
}

void BlockElement::setBorderRight(ed_border_t right) {
    border_right_ = right;
}

void BlockElement::setBorderRightWidth(unsigned short width) {
    border_right_wd_ = width;
}

void BlockElement::setBorderTop(ed_border_t top) {
    border_top_ = top;
}

void BlockElement::setBorderTopWidth(unsigned short width) {
    border_top_wd_ = width;
}

}
