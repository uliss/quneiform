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

namespace cf
{

BlockElement::BlockElement(BlockElement * parent) :
    Element(parent), border_left_(ED_BRDR_NONE), border_top_(ED_BRDR_NONE), border_right_(
            ED_BRDR_NONE), border_bottom_(ED_BRDR_NONE), border_left_wd_(0), border_top_wd_(0),
    border_right_wd_(0), border_bottom_wd_(0) {
}

BlockElement::BlockElement(const BlockElement& el) :
    Element(el)
{
    border_left_ = el.border_left_;
    border_top_ = el.border_top_;
    border_right_ = el.border_right_;
    border_bottom_ = el.border_bottom_;
    border_left_wd_ = el.border_left_wd_;
    border_top_wd_ = el.border_top_wd_;
    border_right_wd_ = el.border_right_wd_;
    border_bottom_wd_ = el.border_bottom_wd_;
    margins_ = el.margins_;

    elements_.reserve(el.elementCount());
    for(size_t i = 0; i < el.elementCount(); i++) {
        elements_.push_back(ElementPtr(el.elementAt(i)->clone()));
        elements_.back()->setParent(this);
    }
}

Element * BlockElement::elementAt(size_t pos)
{
    return elements_.at(pos).get();
}

const Element * BlockElement::elementAt(size_t pos) const
{
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

void BlockElement::exportChildren(CEDExporter& exp) {
    for (iterator it = elements_.begin(), e = elements_.end(); it != e; ++it)
        (*it)->exportElement(exp);
}

void BlockElement::addElement(Element * e) {
    addElement(ElementPtr(e));
}

void BlockElement::addElement(ElementPtr e) {
    elements_.push_back(e);
    e->setParent(this);
}

bool BlockElement::hasElement(Element * el) {
    for (iterator it = elements_.begin(), e = elements_.end(); it != e; ++it) {
        if(it->get() == el)
            return true;
    }

    return false;
}

Element * BlockElement::lastElement() {
    if (elements_.empty())
        throw std::out_of_range("[BlockElement::lastElement] element is empty");
    return elements_.back().get();
}

const Element * BlockElement::lastElement() const {
    if (elements_.empty())
        throw std::out_of_range("[BlockElement::lastElement] element is empty");
    return elements_.back().get();
}

int BlockElement::marginBottom() const {
    return margins_.bottom();
}

int BlockElement::marginLeft() const {
    return margins_.left();
}

int BlockElement::marginRight() const {
    return margins_.right();
}

int BlockElement::marginTop() const {
    return margins_.top();
}

Rect BlockElement::margins() const {
    return margins_;
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

void BlockElement::setMargins(const Rect& margins) {
    margins_ = margins;
}

void BlockElement::setMargins(int top, int right, int bottom, int left) {
    margins_.setLeftTop(left, top);
    margins_.setRightBottom(right, bottom);
}

void BlockElement::setMarginBottom(int bottom) {
    margins_.setBottom(bottom);
}

void BlockElement::setMarginTop(int top) {
    margins_.setTop(top);
}

void BlockElement::updateBoundingRect() {
    for (iterator it = elements_.begin(), end = elements_.end(); it != end; ++it) {
        (*it)->updateBoundingRect();
        Rect brect = (*it)->boundingRect();

        if (brect == Rect(Point(-1, -1), Point(-1, -1)))
            continue;

        if (it == elements_.begin())
            boundingRect() = brect;
        else
            boundingRect() |= brect;
    }
}

}
