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

#include <sstream>
#include <cstdio>
#include <iomanip>
#include "common/font.h"
#include "cssexporter.h"
#include "ced/cedchar.h"

namespace CIF
{

inline std::string color2css(const Color& c) {
    char buf[7];
    snprintf(buf, sizeof(buf), "%02x%02x%02x", c.red(), c.green(), c.blue());
    return buf;
}

CssExporter::CssExporter(CEDPage * page, const FormatOptions& opts) :
    StyleExporter(page, opts) {

}

void CssExporter::addStyle(const CEDChar& chr, size_t hash) {
    styles_.insert(std::make_pair(makeCssStyleKey(hash), makeCssStyle(chr)));
    StyleExporter::addStyle(chr, hash);
}

std::string CssExporter::makeCssStyle(const CEDChar& chr) const {
    std::ostringstream buf;

    if (!chr.color().isNull())
        buf << "    color: #" << color2css(chr.color()) << ";\n";

    if (!chr.backgroundColor().isNull())
        buf << "    background-color: #" << color2css(chr.backgroundColor()) << ";\n";

    if (formatOptions().isFontSizeUsed() && chr.fontHeight() > 0)
        buf << "    font-height: " << chr.fontHeight() << "px;\n";

    return buf.str();
}

std::string CssExporter::makeCssStyleKey(size_t hash) const {
    std::ostringstream buf;
    buf << "char_" << std::hex << hash;
    return buf.str();
}

void CssExporter::writePageEnd(std::ostream& os, CEDPage&) {
    os << "/* cuneiform css style */\n";
    for (StyleMap::iterator it = styles_.begin(); it != styles_.end(); ++it)
        writeStyleEntry(os, *it);
}

void CssExporter::writeStyleEntry(std::ostream& os, const StyleEntry& entry) {
    os << "." << entry.first << " {\n" << entry.second << "}\n";
}

}
