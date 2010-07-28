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

#include <iomanip>
#include "djvuxmlexporter.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "common/tostring.h"
#include "common/helper.h"

namespace CIF
{

static const int DJVU_XML_INDENT = 2;
static const std::string DJVUXML_DOCTYPE(
        "<!DOCTYPE DjVuXML PUBLIC \"-//W3C//DTD DjVuXML 1.1//EN\" \"pubtext/DjVuXML-s.dtd\">");

inline bool isSpace(const CEDChar& chr) {
    return chr.boundingRect() == Rect(Point(-1, -1), Point(-1, -1));
}

DjvuTxtExporter::DjvuTxtExporter(CEDPage * page, const FormatOptions& opts) :
    XmlExporter(page, opts), indent_level_(0), word_flag_(false) {
    formatOptions().setPreserveLineBreaks(true);
}

void DjvuTxtExporter::calcWordRectList(CEDLine& line) {
    word_rects_.clear();
    Rect word_rect;
    bool word_flag = false;

    for (size_t i = 0; i < line.elementCount(); i++) {
        CEDChar * chr = dynamic_cast<CEDChar*> (line.elementAt(i));
        if (chr && !isSpace(*chr)) {
            if (word_flag) {
                word_rect |= chr->boundingRect();
            } else {
                word_flag = true;
                word_rect = chr->boundingRect();
            }
        } else {
            word_flag = false;
            word_rects_.push_back(word_rect);
            word_rect = Rect();
        }
    }

    if (word_flag)
        word_rects_.push_back(word_rect);
}

void DjvuTxtExporter::closeWord() {
    word_flag_ = false;
    writeWordEnd();
}

void DjvuTxtExporter::setCoordAttr(Attributes& attrs, const Rect& r) {
    std::ostringstream buf;
    buf << r.left() << ',' << r.top() << ',' << r.right() << ',' << r.bottom();
    attrs["coords"] = buf.str();
}

void DjvuTxtExporter::startWord() {
    word_flag_ = true;
    writeWordBegin();
}

void DjvuTxtExporter::writeCharacter(CEDChar& chr) {
    if (isSpace(chr))
        return;

    TextExporter::writeCharacter(chr);
}

void DjvuTxtExporter::writeCharacterBegin(CEDChar& chr) {
    if (isSpace(chr)) {
        if (word_flag_)
            closeWord();

        return;
    } else {
        if (!word_flag_)
            startWord();
    }
}

void DjvuTxtExporter::writeColumnBegin(CEDChar&) {
    indent_level_++;
    writeIndent();
    writeStartTag("PAGECOLUMN", "\n");
}

void DjvuTxtExporter::writeColumnEnd(CEDChar&) {
    writeIndent();
    writeCloseTag("PAGECOLUMN", "\n");
    indent_level_--;
}

void DjvuTxtExporter::writeIndent() {
    if (indent_level_ > 0)
        buffer() << std::string(indent_level_ * DJVU_XML_INDENT, ' ');
}

void DjvuTxtExporter::writeLineBegin(CEDLine& line) {
    calcWordRectList(line);
    indent_level_++;
    writeIndent();
    writeStartTag("LINE", "\n");
}

void DjvuTxtExporter::writeLineBreak() {

}

void DjvuTxtExporter::writeLineEnd(CEDLine&) {
    if (word_flag_)
        closeWord();

    writeIndent();
    writeCloseTag("LINE", "\n");
    indent_level_--;
}

void DjvuTxtExporter::writeParagraphBegin(CEDParagraph&) {
    indent_level_++;
    writeIndent();
    writeStartTag("PARAGRAPH", "\n");
}

void DjvuTxtExporter::writeParagraphEnd(CEDParagraph&) {
    writeIndent();
    writeCloseTag("PARAGRAPH", "\n");
    indent_level_--;
}

void DjvuTxtExporter::writePageBegin(CEDPage& page) {
    writeXmlDeclaration(buffer());
    writeStartTag("DjVuXML", "\n");
    writeTag("HEAD", "", Attributes(), "\n");
    writeStartTag("BODY", "\n");

    Attributes obj_attrs;
    obj_attrs["data"] = escapeHtmlSpecialChars(page.imageName());
    obj_attrs["height"] = toString(page.imageSize().height());
    obj_attrs["width"] = toString(page.imageSize().width());
    obj_attrs["type"] = "image/x.djvu";
    writeStartTag("OBJECT", obj_attrs, "\n");
    indent_level_++;
    writeIndent();
    writeStartTag("HIDDENTEXT", "\n");
}

void DjvuTxtExporter::writePageEnd(CEDPage&) {
    writeIndent();
    indent_level_--;
    writeCloseTag("HIDDENTEXT", "\n");
    writeCloseTag("OBJECT", "\n");
    writeCloseTag("BODY", "\n");
    writeCloseTag("DjVuXML", "\n");
    flushBuffer();
}

void DjvuTxtExporter::writeSectionBegin(CEDSection&) {
    indent_level_++;
    writeIndent();
    writeStartTag("REGION", "\n");
}

void DjvuTxtExporter::writeSectionEnd(CEDSection&) {
    writeIndent();
    writeCloseTag("REGION", "\n");
    indent_level_--;
}

void DjvuTxtExporter::writeWordBegin() {
    Attributes word_attrs;
    setCoordAttr(word_attrs, word_rects_.front());
    word_rects_.pop_front();

    indent_level_++;
    writeIndent();
    writeStartTag("WORD", word_attrs);
}

void DjvuTxtExporter::writeWordEnd() {
    writeCloseTag("WORD", "\n");
    indent_level_--;
}

}
