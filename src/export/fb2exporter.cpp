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

#include <fstream>
#include <sstream>

#include "fb2exporter.h"
#include "ced/cedpage.h"

namespace cf {

FB2Exporter::FB2Exporter(cf::CEDPage * page, const cf::FormatOptions& opts)
    : XmlExporter(page, opts)
{
}

void FB2Exporter::writeBinary() {
    writeStartTag("binary", "\n");
    writeCloseTag("binary", "\n");
}

void FB2Exporter::writeDescription() {
    writeStartTag("description", "\n");
    writeDocumentTitle();
    writeDocumentInfo();
    writeCloseTag("description", "\n");
}

void FB2Exporter::writeDocumentInfo() {
    writeStartTag("document-info", "\n");
    writeCloseTag("document-info", "\n");
}

void FB2Exporter::writeDocumentTitle() {
    writeStartTag("title-info", "\n");
    writeCloseTag("title-info", "\n");
}

void FB2Exporter::writePageBegin(CEDPage& page) {
    writeXmlDeclaration(*outputStream());
    XmlExporter::Attributes attrs;
    attrs["xmlns"] = "http://www.gribuser.ru/xml/fictionbook/2.0";
    attrs["xmlns:l"] = "http://www.w3.org/1999/xlink";
    writeStartTag("FictionBook", attrs, "\n");

    writeDescription();
    writeStartTag("body", "\n");
}

void FB2Exporter::writePageEnd(CEDPage &page) {
    writeCloseTag("body", "\n");

    writeBinary();

    writeCloseTag("FictionBook", "\n");
    flushBuffer();
}

void FB2Exporter::writeParagraphBegin(CEDParagraph&) {
    writeStartTag("p");
}

void FB2Exporter::writeParagraphEnd(CEDParagraph&) {
    writeCloseTag("p", "\n");
}

void FB2Exporter::writeSectionBegin(CEDSection& sect) {
    writeStartTag("section", "\n");
}

void FB2Exporter::writeSectionEnd(CEDSection& sect) {
    writeCloseTag("section", "\n");
}

}
