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

#include "edexporter.h"
#include "ced/ced.h"

namespace CIF
{

EdExporter::EdExporter(Handle page) :
    Exporter(FormatOptions()), page_(page) {
}

void EdExporter::exportTo(const std::string& filename) {
    if (!CED_WriteFormattedEd(filename.c_str(), page_))
        throw Exception("Save to native format failed");
}

void EdExporter::doExport(std::ostream&) {
    throw Exception("Export to stream for native format not supported");
}

}
