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

#include "summaryexporter.h"
#include "ced/cedchar.h"
#include "ced/cedcolumn.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
#include "ced/cedpicture.h"

namespace cf
{

SummaryExporter::SummaryExporter(CEDPagePtr page, const FormatOptions& opts) :
    GenericExporter(page, opts) {
    setSkipPictures(false);
    setSkipEmptyLines(false);
    setSkipEmptyParagraphs(false);
}

void SummaryExporter::writePageEnd(CEDPage&) {
    *outputStream() << "##################################\n";
    *outputStream() << "Page summary: " << "\n";
    *outputStream() << "    total sections:   " << numSections() << "\n";
    *outputStream() << "    total columns:    " << numColumns() << "\n";
    *outputStream() << "    total paragraphs: " << numParagraphs() << "\n";
    *outputStream() << "    total frames:     " << numFrames() << "\n";
    *outputStream() << "    total lines:      " << numLines() << "\n";
    *outputStream() << "    total chars:      " << numChars() << "\n";
    *outputStream() << "    total pictures:   " << numPictures() << "\n";
    *outputStream() << "    total tables:     " << numTables() << "\n";
}

}
