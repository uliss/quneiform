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

void StyleExporter::addStyle(const CEDChar&, size_t hash) {
    hashes_.insert(hash);
}

void StyleExporter::exportChar(CEDChar& chr) {
    size_t chr_hash = hash(chr);
    // not found
    if (hashes_.find(chr_hash) == hashes_.end())
        addStyle(chr, chr_hash);
}

size_t StyleExporter::hash(const CEDChar& chr) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, chr.color().toT<int> ());
    boost::hash_combine(seed, chr.backgroundColor().toT<int> ());
    boost::hash_combine(seed, chr.fontStyle());
    return seed;
}

}
