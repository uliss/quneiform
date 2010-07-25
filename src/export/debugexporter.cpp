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
#include "debugexporter.h"
#include "cstr/cstr.h"
#include "common/iconv_local.h"

namespace CIF
{

DebugExporter::DebugExporter(const FormatOptions& opts) :
    Exporter(opts) {
    formatOptions().setPreserveLineBreaks(true);
}

void DebugExporter::appendTo(const std::string& filename) {
    std::ofstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    if (!f)
        throw Exception("[DebugExporter::appendTo] append failed to: " + filename);
    setOutputFilename(filename);
    doExport(f);
}

void DebugExporter::doExport(std::ostream& os) {
    Iconv converter(inputEncoding(), outputEncoding());
    bool do_encode = isCharsetConversion();

    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
        CSTR_line lin_out = CSTR_GetLineHandle(i, 1);
        if (!lin_out)
            throw Exception("CSTR_GetLineHandle failed");

        // uliss: FIXME
        char unrec[2] = { 0, 0 };
        unrec[0] = (char) formatOptions().unrecognizedChar();

        if (do_encode) {
            os << converter.convert(CSTR_LineToTxt(lin_out, unrec));
        } else {
            os << CSTR_LineToTxt(lin_out, unrec);
        }

        if (formatOptions().preserveLineBreaks())
            os << '\n';
        else
            os << ' ';
    }

    os << std::endl;
}

}
