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

#include <boost/functional/hash.hpp>
#include "styleexporter.h"
#include "ced/cedchar.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
#include "common/helper.h"
#include "common/tostring.h"
#include "common/fontstyle.h"

namespace cf
{

StyleExporter::StyleExporter(CEDPagePtr page, const FormatOptions& opts) :
    GenericExporter(page, opts),
    style_num_char_(0),
    style_num_paragraph_(0),
    style_num_section_(0)
{
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

void StyleExporter::addStyle(const std::string& styleName, size_t hash) {
    if (!styleName.empty())
        hashes_[hash] = styleName;
}

void StyleExporter::exportChar(CEDChar& chr) {
    exportElement(chr);
}

void StyleExporter::exportParagraph(CEDParagraph& par) {
    GenericExporter::exportParagraph(par);
    exportElement(par);
}

void StyleExporter::exportSection(CEDSection& s) {
    GenericExporter::exportSection(s);
    exportElement(s);
}

size_t StyleExporter::hash(const CEDChar& chr) const {
    std::size_t seed = 0;

    // colors
    boost::hash_combine(seed, chr.color().toT<int> ());
    boost::hash_combine(seed, chr.backgroundColor().toT<int> ());

    // bold
    if (formatOptions().isBoldUsed() && (chr.fontStyle() & FONT_BOLD)) {
        boost::hash_combine(seed, static_cast<int> (FONT_BOLD));
    }

    // italic
    if (formatOptions().isItalicUsed() && (chr.fontStyle() & FONT_ITALIC)) {
        boost::hash_combine(seed, static_cast<int> (FONT_ITALIC));
    }

    // underlined
    if (chr.fontStyle() & FONT_UNDERLINE) {
        boost::hash_combine(seed, static_cast<int> (FONT_UNDERLINE));
    }

    // superscript
    if (chr.fontStyle() & FONT_SUPER) {
        boost::hash_combine(seed, static_cast<int> (FONT_SUPER));
    }

    // superscript
    if (chr.fontStyle() & FONT_SUB) {
        boost::hash_combine(seed, static_cast<int> (FONT_SUB));
    }

    // strike
    if (chr.fontStyle() & FONT_STRIKE) {
        boost::hash_combine(seed, static_cast<int> (FONT_STRIKE));
    }

    // font size
    if (formatOptions().isFontSizeUsed()) {
        boost::hash_combine(seed, chr.fontHeight());
    }

    return seed;
}

size_t StyleExporter::hash(const CEDParagraph& par) const {
    size_t seed = 1;
    boost::hash_combine(seed, par.color().toT<int> ());
    boost::hash_combine(seed, par.backgroundColor().toT<int> ());
    boost::hash_combine(seed, static_cast<int>(par.align()));
    boost::hash_combine(seed, par.indent());
    return seed;
}

size_t StyleExporter::hash(const CEDSection& sec) const {
    size_t seed = 2;
    boost::hash_combine(seed, sec.color().toT<int> ());
    boost::hash_combine(seed, sec.backgroundColor().toT<int> ());
    boost::hash_combine(seed, sec.columnCount());
    return seed;
}

std::string StyleExporter::makeStyle(const CEDChar&) {
    return "char_" + toString(++style_num_char_);
}

std::string StyleExporter::makeStyle(const CEDParagraph&) {
    return "par_" + toString(++style_num_paragraph_);
}

std::string StyleExporter::makeStyle(const CEDSection&) {
    return "sec_" + toString(++style_num_section_);
}
}
