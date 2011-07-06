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
#include "ced/cedchar.h"
#include "export/rout_own.h"

namespace cf {

FB2Exporter::FB2Exporter(cf::CEDPage * page, const cf::FormatOptions& opts)
    : XmlExporter(page, opts),
    prev_char_style_(0)
{
}

void FB2Exporter::changeCharacterFontStyle(int new_style) {
    writeFontStyleEnd(prev_char_style_);
    writeFontStyleBegin(new_style);
    prev_char_style_ = new_style;
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
    writeTag("program-used", "cuneiform");
    writeCloseTag("document-info", "\n");
}

void FB2Exporter::writeDocumentTitle() {
    writeStartTag("title-info", "\n");
    writeCloseTag("title-info", "\n");
}

void FB2Exporter::writeCharacterBegin(CEDChar& c) {
    // font style changed
    if (prev_char_style_ != c.fontStyle())
        changeCharacterFontStyle(c.fontStyle());
}

void FB2Exporter::writeFontStyleBegin(int style) {
    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        writeStartTag("strong");

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        writeStartTag("emphasis");

    if (formatOptions().isUnderlinedUsed() && style & FONT_UNDERLINE)
        std::cerr << "[Warning] FB2 format has no underlined text support" << std::endl;

    if (style & FONT_SUB)
        writeStartTag("sub");

    if (style & FONT_SUPER)
        writeStartTag("sup");
}

void FB2Exporter::writeFontStyleEnd(int style) {
    if (style & FONT_SUPER)
        writeCloseTag("sup");

    if (style & FONT_SUB)
        writeCloseTag("sub");

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        writeCloseTag("emphasis");

    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        writeCloseTag("strong");
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
    prev_char_style_ = 0;
}

void FB2Exporter::writeParagraphEnd(CEDParagraph&) {
    writeFontStyleEnd(prev_char_style_);
    writeCloseTag("p", "\n");
}

void FB2Exporter::writeSectionBegin(CEDSection& sect) {
    writeStartTag("section", "\n");
}

void FB2Exporter::writeSectionEnd(CEDSection& sect) {
    writeCloseTag("section", "\n");
}

}
