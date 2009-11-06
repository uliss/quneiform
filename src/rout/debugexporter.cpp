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

#include "debugexporter.h"
#include "cstr/cstr.h"

namespace CIF {

DebugExporter::DebugExporter(const FormatOptions& opts) :
    Exporter(opts) {

}

DebugExporter::~DebugExporter() {
}

void DebugExporter::exportPage(std::ostream& os) {
    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
        CSTR_line lin_out = CSTR_GetLineHandle(i, 1);
        if (!lin_out)
            throw Exception("CSTR_GetLineHandle failed");

        os << CSTR_LineToTxt(lin_out, formatOptions().unrecognizedChar());
        if(formatOptions().preserveLineBreaks())
            os << "\n";
    }
}

}
