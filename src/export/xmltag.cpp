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

#include "xmltag.h"
#include "common/helper.h"

namespace CIF
{

XmlTag::XmlTag(const std::string& name, const std::string& content) :
    Tag(name, content) {

}

XmlTag::XmlTag(const std::string& name, const Attributes& attrs) :
    Tag(name, attrs) {

}

void XmlTag::writeAttributes(std::ostream& os) const {
    for (Attributes::const_iterator it = attrs_.begin(), end = attrs_.end(); it != end; ++it)
        os << " " << it->first << "=\"" << it->second << "\"";
}

void XmlTag::writeBegin(std::ostream& os) const {
    os << "<" << name();
    writeAttributes(os);
    os << ">";
}

void XmlTag::writeContent(std::ostream& os) const {
    os << escapeHtmlSpecialChars(content());
}

void XmlTag::writeEnd(std::ostream& os) const {
    os << "</" << name() << ">";
}

void XmlTag::writeSingle(std::ostream& os) const {
    os << "<" << name();
    writeAttributes(os);
    os << "/>";
}

}
