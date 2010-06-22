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

#include "element.h"

namespace CIF
{

Element::Element(Element * parent) :
    parent_(parent), color_(Color::null()), bgcolor_(Color::null()) {

}

Element::~Element() {
}

const Color & Element::backgroundColor() const {
    return bgcolor_;
}

const Rect& Element::boundingRect() const {
    return brect_;
}

const Color & Element::color() const {
    return color_;
}

Element * Element::parent() {
    return parent_;
}

const Element * Element::parent() const {
    return parent_;
}

void Element::setBackgroundColor(const Color & c) {
    bgcolor_ = c;
}

void Element::setBoundingRect(const Rect& r) {
    brect_ = r;
}

void Element::setColor(const Color & c) {
    color_ = c;
}

void Element::setParent(Element * parent) {
    parent_ = parent;
}

}
