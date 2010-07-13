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
#include "ced/cedparagraph.h"

namespace CIF
{

inline std::string color2css(const Color& c) {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", c.red(), c.green(), c.blue());
    return buf;
}

inline std::string align2css(align_t align) {
    switch (align) {
    case ALIGN_CENTER:
        return "center";
    case ALIGN_RIGHT:
        return "right";
    case ALIGN_JUSTIFY:
        return "justify";
    default:
        return "";
    }
}

inline void writeCssEntry(std::ostream& os, const std::string& key, const std::string& value) {
    if (value.empty())
        return;

    os << "    " << key << ": " << value << ";\n";
}

inline void writeCssColor(std::ostream& os, const std::string& key, const Color& color) {
    if (color.isNull())
        return;
    writeCssEntry(os, key, color2css(color));
}

inline void writeCssEntryPx(std::ostream& os, const std::string& key, int value) {
    os << "    " << key << ": " << value << "px;\n";
}

CssExporter::CssExporter(CEDPage * page, const FormatOptions& opts) :
    StyleExporter(page, opts) {
}

std::string CssExporter::makeCssStyle(const CEDChar& chr) const {
    std::ostringstream buf;

    writeCssColor(buf, "color", chr.color());
    writeCssColor(buf, "background-color", chr.backgroundColor());

    if (formatOptions().isFontSizeUsed() && chr.fontHeight() > 0)
        writeCssEntryPx(buf, "font-height", chr.fontHeight());

    if (formatOptions().isBoldUsed() && chr.fontStyle() & FORMAT_FONT_BOLD)
        writeCssEntry(buf, "font-weight", "bold");

    if (formatOptions().isItalicUsed() && chr.fontStyle() & FORMAT_FONT_ITALIC)
        writeCssEntry(buf, "font-decoration", "italic");

    return buf.str();
}

std::string CssExporter::makeCssStyle(const CEDParagraph& par) const {
    std::ostringstream buf;

    writeCssColor(buf, "color", par.color());
    writeCssColor(buf, "background-color", par.backgroundColor());
    writeCssEntry(buf, "text-align", align2css(par.align()));
    if (par.indent() > 0)
        writeCssEntryPx(buf, "text-indent", par.indent());

    return buf.str();
}

std::string CssExporter::makeStyle(const CEDChar& chr) {
    std::string chr_style = StyleExporter::makeStyle(chr);
    styles_[chr_style] = makeCssStyle(chr);
    return chr_style;
}

std::string CssExporter::makeStyle(const CEDParagraph& par) {
    std::string par_style = StyleExporter::makeStyle(par);
    styles_[par_style] = makeCssStyle(par);
    return par_style;
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
