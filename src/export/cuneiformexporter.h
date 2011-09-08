/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef CUNEIFORMEXPORTER_H
#define CUNEIFORMEXPORTER_H

#include "globus.h"
#include "exporter.h"

namespace cf {

class CLA_EXPO CuneiformExporter : public Exporter
{
public:
    CuneiformExporter(CEDPagePtr page, const FormatOptions& opts);
private:
    void doExport(std::ostream& os);
    void exportChar(CEDChar&) {}
    void exportColumn(CEDColumn&) {}
    void exportFrame(CEDFrame&) {}
    void exportLine(CEDLine&) {}
    void exportPage(CEDPage&) {}
    void exportParagraph(CEDParagraph&) {}
    void exportPicture(CEDPicture&) {}
    void exportSection(CEDSection&) {}
    void exportTable(CEDTable&) {}
};

}

#endif // CUNEIFORMEXPORTER_H
