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

#include "textexporter.h"
#include "rout.h"

#include <cstdlib>
#include <cstring>

namespace CIF {

TextExporter::TextExporter(Handle page, int format, const FormatOptions& opt) :
    Exporter(opt), format_(format), page_(page) {
}

TextExporter::~TextExporter() {
}

void TextExporter::exportTo(const std::string& filename) {
    char szName[260];
    strcpy(szName, filename.c_str());
    char * str = strrchr(szName, '.');
    if (str)
        *(str) = '\0';

    Bool line_breaks = formatOptions().preserveLineBreaks();
    char unrecog = formatOptions().unrecognizedChar();
    if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks, (void*) line_breaks)
            || !ROUT_SetImportData(ROUT_PCHAR_PageName, szName) || !ROUT_SetImportData(
            ROUT_HANDLE_PageHandle, page_)
            || !ROUT_SetImportData(ROUT_LONG_Format, (void*) format_) || !ROUT_SetImportData(
            ROUT_LONG_Code, (void*) PUMA_CODE_UTF8) || !ROUT_SetImportData(ROUT_PCHAR_BAD_CHAR,
            (void*) &unrecog))
        throw Exception("ROUT_SetImportData failed");

    // Количество объектов
    long countObjects = ROUT_CountObjects();
    if (countObjects == -1)
        return;

    // Цикл по объектам на странице
    for (long objIndex = 1; objIndex <= countObjects; objIndex++) {
        std::string path(filename);

        if (countObjects != 1) {
            path = ROUT_GetDefaultObjectName(objIndex);
            if (!path.empty())
                throw Exception("ROUT_GetDefaultObjectName failed");
        }

        if (!ROUT_SaveObject(objIndex, path.c_str(), FALSE))
            throw Exception("ROUT_SaveObject failed");
    }
}

void TextExporter::doExport(std::ostream&) {
    throw Exception("Export to stream for text format not supported yet");
}

}
