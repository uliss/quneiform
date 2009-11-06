/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <fstream>

#include "exporter.h"

using namespace std;

namespace CIF {

Exporter::Exporter() {
}

Exporter::Exporter(const FormatOptions& opts) :
    format_options_(opts) {
}

Exporter::~Exporter() {
}

void Exporter::exportTo(const std::string& filename) {
    ofstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!f)
        throw Exception("Can't write to file: " + filename);
    exportTo(f);
}

void Exporter::exportTo(std::ostream& os) {
    exportPage(os);
}

FormatOptions Exporter::formatOptions() const {
    return format_options_;
}

void Exporter::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
}

}
