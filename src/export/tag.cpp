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

#include "tag.h"

namespace CIF
{

Tag::Tag(const std::string& name, const std::string& content) :
    tag_name_(name), content_(content) {

}

Tag::Tag(const std::string& name, const Attributes& attrs) :
    tag_name_(name), attrs_(attrs) {

}

Tag::~Tag() {
}

bool Tag::hasAttribute(const std::string& name) const {
    return attrs_.find(name) != attrs_.end();
}

std::string Tag::attribute(const std::string& name) const {
    Attributes::const_iterator it = attrs_.find(name);
    return it == attrs_.end() ? "" : it->second;
}

void Tag::clear() {
    content_.clear();
    attrs_.clear();
}

std::string Tag::content() const {
    return content_;
}

bool Tag::isSingle() const {
    return content_.empty();
}

void Tag::setAttribute(const std::string& name, const std::string& value) {
    attrs_[name] = value;
}

void Tag::setAttributes(const Attributes& attrs) {
    attrs_ = attrs;
}

void Tag::setContent(const std::string& content) {
    content_ = content;
}

void Tag::setName(const std::string& tagName) {
    tag_name_ = tagName;
}

void Tag::writeBeginNL(std::ostream& os) const {
    writeBegin(os);
    os << "\n";
}

void Tag::writeEndNL(std::ostream& os) const {
    writeEnd(os);
    os << "\n";
}

std::ostream& operator<<(std::ostream& os, const Tag& tag) {
    if (tag.isSingle()) {
        tag.writeSingle(os);
    } else {
        tag.writeBegin(os);
        tag.writeContent(os);
        tag.writeEnd(os);
    }
    return os;
}

}
