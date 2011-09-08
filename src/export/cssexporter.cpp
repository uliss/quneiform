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
#include <boost/functional/hash.hpp>

#include "cssexporter.h"
#include "cfcompat.h"
#include "common/font.h"
#include "ced/cedchar.h"
#include "ced/cedparagraph.h"

namespace cf
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

CssExporter::CssExporter(CEDPagePtr page, const FormatOptions& opts) :
    StyleExporter(page, opts) {
}

void CssExporter::addCssStyle(const std::string& name, const std::string& content) {
    static size_t seed = 1000;
    boost::hash_combine(seed, seed);
    addStyle(name, seed);
    styles_[name] = content;
}

size_t CssExporter::hash(const CEDChar& chr) const {
    size_t seed = 1;
    boost::hash_combine(seed, chr.color().toT<int> ());
    boost::hash_combine(seed, chr.backgroundColor().toT<int> ());
    if (formatOptions().isFontSizeUsed())
        boost::hash_combine(seed, chr.fontHeight());
    return seed;
}

size_t CssExporter::hash(const CEDParagraph& par) const {
    size_t seed = 2;
    boost::hash_combine(seed, par.color().toT<int> ());
    boost::hash_combine(seed, par.backgroundColor().toT<int> ());
    boost::hash_combine(seed, par.lineSpace());
    boost::hash_combine(seed, par.indent());
    return seed;
}

std::string CssExporter::makeCssStyle(const CEDChar& chr) const {
    std::ostringstream buf;

    writeCssColor(buf, "color", chr.color());
    writeCssColor(buf, "background-color", chr.backgroundColor());

    if (formatOptions().isFontSizeUsed() && chr.fontHeight() > 0)
        writeCssEntryPx(buf, "font-size", chr.fontHeight());

    return buf.str();
}

std::string CssExporter::makeCssStyle(const CEDParagraph& par) const {
    std::ostringstream buf;

    writeCssColor(buf, "color", par.color());
    writeCssColor(buf, "background-color", par.backgroundColor());

    if (par.lineSpace() > 0)
        writeCssEntryPx(buf, "line-height", par.lineSpace());

    if (par.indent() != 0)
        writeCssEntryPx(buf, "text-indent", par.indent());

    return buf.str();
}

std::string CssExporter::makeStyle(const CEDChar& chr) {
    std::string content = makeCssStyle(chr);
    if (content.empty())
        return "";

    std::string chr_style = StyleExporter::makeStyle(chr);
    styles_[chr_style] = content;
    return chr_style;
}

std::string CssExporter::makeStyle(const CEDParagraph& par) {
    std::string content = makeCssStyle(par);
    if (content.empty())
        return "";

    std::string par_style = StyleExporter::makeStyle(par);

    styles_[par_style] = content;
    return par_style;
}

void CssExporter::writePageEnd(CEDPage&) {
    assert(outputStream());

    *outputStream() << "/* cuneiform css style */\n";
    for (StyleMap::iterator it = styles_.begin(); it != styles_.end(); ++it)
        writeStyleEntry(*it);
}

void CssExporter::writeStyleEntry(const StyleEntry& entry) {
    assert(outputStream());

    *outputStream() << "." << entry.first << " {\n" << entry.second << "}\n";
}

}
