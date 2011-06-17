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

#ifndef CUNEIFORMXMLEXPORTER_H
#define CUNEIFORMXMLEXPORTER_H

#include "globus.h"
#include "exporter.h"

namespace cf {

class CLA_EXPO CuneiformXmlExporter : public Exporter
{
public:
    CuneiformXmlExporter(CEDPage * page, const FormatOptions& opts);
    CEDPage * page();
    void setPage(CEDPage * page);
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
private:
    CEDPage * page_;
};

}

#endif // CUNEIFORMXMLEXPORTER_H
