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
#include <algorithm>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include "fb2exporter.h"
#include "common/debug.h"
#include "common/helper.h"
#include "common/fontstyle.h"
#include "common/language.h"
#include "ced/cedpage.h"
#include "ced/cedchar.h"
#include "export/imageexporterfactory.h"

namespace cf {

FB2Exporter::FB2Exporter(CEDPagePtr page, const cf::FormatOptions& opts)
    : XmlExporter(page, opts),
    prev_char_style_(0)
{
    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);
}

void FB2Exporter::changeCharacterFontStyle(int new_style) {
    writeFontStyleEnd(prev_char_style_);
    writeFontStyleBegin(new_style);
    prev_char_style_ = new_style;
}

void FB2Exporter::writeBinary() {
    writePictures();
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

void FB2Exporter::writeDocumentLanguage() {
    writeTag("lang", Language::isoCode2(formatOptions().language()), Attributes(), "\n");
}

void FB2Exporter::writeDocumentTitle() {
    writeStartTag("title-info", "\n");
    writeDocumentLanguage();
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
        Debug() << "[Warning] FB2 format has no underlined text support" << std::endl;

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

void FB2Exporter::writeParagraphBegin(CEDParagraph& p) {
    XmlExporter::writeParagraphBegin(p);
    writeStartTag("p");
    prev_char_style_ = 0;
}

void FB2Exporter::writeParagraphEnd(CEDParagraph&) {
    writeFontStyleEnd(prev_char_style_);
    writeCloseTag("p", "\n");
}

void FB2Exporter::writePicture(CEDPicture& picture) {
    Attributes attrs;
    attrs["l:href"] = "#" + escapeHtmlSpecialChars(makePictureName(picture));
    writeSingleTag("image", attrs);

    pictures_.push_back(&picture);
}

void FB2Exporter::writePictureBase64(const CEDPicture& p) {
    std::stringstream buf;
    savePictureData(p, buf);

    typedef std::istreambuf_iterator<char> istream_iterator;
    typedef std::ostreambuf_iterator<char> ostream_iterator;

    using namespace boost::archive::iterators;
    typedef insert_linebreaks<
                base64_from_binary<
                    transform_width<
                        istream_iterator, 6, 8
                    >
                >,
                72>
    base64_text;

    std::copy(
            base64_text(istream_iterator(buf)),
            base64_text(istream_iterator()),
            ostream_iterator(buffer())
    );
}

void FB2Exporter::writePictures() {
    try {
        for(PictList::iterator it = pictures_.begin(); it != pictures_.end(); ++it) {
            if(*it == NULL)
                continue;

            CEDPicture& p = *(*it);
            Attributes attrs;
            attrs["id"] = makePictureName(p);
            writeStartTag("binary", attrs);
            writePictureBase64(p);
            writeCloseTag("binary", "\n");
        }
    } catch (Exception& e) {
        Debug() << "[FB2Exporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void FB2Exporter::writeSectionBegin(CEDSection& sect) {
    writeStartTag("section", "\n");
}

void FB2Exporter::writeSectionEnd(CEDSection& sect) {
    writeCloseTag("section", "\n");
}

}
