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
#include "common/helper.h"
#include "common/tostring.h"
#include "common/font.h"

namespace CIF
{

StyleExporter::StyleExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

StyleExporter::~StyleExporter() {

}

void StyleExporter::addStyle(const std::string& name, size_t hash) {
    hashes_[hash] = name;
}

void StyleExporter::exportChar(CEDChar& chr) {
    size_t chr_hash = hash(chr);
    // not found
    if (hashes_.find(chr_hash) == hashes_.end())
        addStyle(makeStyle(chr), chr_hash);
}

void StyleExporter::exportParagraph(CEDParagraph& par) {
    GenericExporter::exportParagraph(par);
    size_t par_hash = hash(par);
    if (hashes_.find(par_hash) == hashes_.end())
        addStyle(makeStyle(par), par_hash);
}

size_t StyleExporter::hash(const CEDChar& chr) const {
    std::size_t seed = 0;

    // colors
    boost::hash_combine(seed, chr.color().toT<int> ());
    boost::hash_combine(seed, chr.backgroundColor().toT<int> ());

    // bold
    if (formatOptions().isBoldUsed() && (chr.fontStyle() & FORMAT_FONT_BOLD)) {
        boost::hash_combine(seed, static_cast<int> (FORMAT_FONT_BOLD));
    }

    // italic
    if (formatOptions().isItalicUsed() && (chr.fontStyle() & FORMAT_FONT_ITALIC)) {
        boost::hash_combine(seed, static_cast<int> (FORMAT_FONT_ITALIC));
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
    boost::hash_combine(seed, par.align());
    boost::hash_combine(seed, par.indent());
    return seed;
}

std::string StyleExporter::makeStyle(const CEDChar&) {
    static int num = 1;
    return "char_" + toString(num++);
}

std::string StyleExporter::makeStyle(const CEDParagraph&) {
    static int num = 1;
    return "par_" + toString(num++);
}

}
