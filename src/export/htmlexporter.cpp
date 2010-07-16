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

#include <cassert>
#include <algorithm>
#include "htmlexporter.h"
#include "common/helper.h"
#include "common/debug.h"
#include "common/font.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "rout_own.h"
#include "xmltag.h"
#include "imageexporterfactory.h"
#include "config.h" // for CF_VERSION
namespace CIF
{

const std::string HTML_ALTERNATIVE_STYLE_CLASS("has_alternative");

const std::string HTML_DOCTYPE("<!DOCTYPE html PUBLIC "
    "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    XmlExporter(page, opts), prev_char_style_(0), close_style_(false) {

    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);
    //    useIndents(true);
}

void HtmlExporter::closeStyle(bool value) {
    close_style_ = value;
}

void HtmlExporter::writeAlternativesBegin(const CEDChar& chr) {
    if (isShowAlternatives() && chr.alternativeCount() > 1) {
        lineBuffer() << "<span title=\"Alternatives:";
        for (size_t i = 1; i < chr.alternativeCount(); i++)
            lineBuffer() << " " << escapeSpecialChar(chr.alternativeAt(i).getChar());

        lineBuffer() << "\" class=\"" << HTML_ALTERNATIVE_STYLE_CLASS << "\">";
    }
}

void HtmlExporter::writeAlternativesEnd(const CEDChar& chr) {
    if (isShowAlternatives() && chr.alternativeCount() > 1)
        lineBuffer() << "</span>";
}

void HtmlExporter::writeCharacterBegin(std::ostream&, CEDChar& chr) {
    if (prev_char_style_ != chr.fontStyle()) {
        writeFontStyleEnd(prev_char_style_);
        writeFontStyleBegin(chr.fontStyle());
        prev_char_style_ = chr.fontStyle();
    }

    writeAlternativesBegin(chr);
}

void HtmlExporter::writeCharacterEnd(std::ostream&, CEDChar& chr) {
    writeAlternativesEnd(chr);
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << HTML_DOCTYPE;
}

void HtmlExporter::writeFontStyleBegin(int style) {
    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        lineBuffer() << "<b>";

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        lineBuffer() << "<i>";

    if (style & FONT_UNDERLINE)
        lineBuffer() << "<u>";

    if (style & FONT_SUB)
        lineBuffer() << "<sub>";

    if (style & FONT_SUPER)
        lineBuffer() << "<sup>";
}

void HtmlExporter::writeFontStyleEnd(int style) {
    if (style & FONT_SUPER)
        lineBuffer() << "</sup>";

    if (style & FONT_SUB)
        lineBuffer() << "</sub>";

    if (style & FONT_UNDERLINE)
        lineBuffer() << "</u>";

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        lineBuffer() << "</i>";

    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        lineBuffer() << "</b>";
}

void HtmlExporter::writeFrameBegin(std::ostream& os, CEDFrame& frame) {
    os << "<div class=\"frame\">";
}

void HtmlExporter::writeFrameEnd(std::ostream& os, CEDFrame& frame) {
    os << "</div>\n";
}

void HtmlExporter::writeLineBreak(std::ostream& os) {
    // skip last line break
    if (lineLeftInParagraph() <= 1)
        return;

    os << XmlTag("br") << "\n";
}

void HtmlExporter::writeMeta(std::ostream& os) {
    XmlTag meta_generator("meta");
    meta_generator["name"] = "Generator";
    meta_generator["content"] = "cuneiform-linux-" CF_VERSION;
    os << meta_generator << "\n";

    if (isCharsetConversion()) {
        XmlTag meta_charset("meta");
        meta_charset["name"] = "Content-Type";
        meta_charset["content"] = "text/html;charset=" + outputEncoding();
        os << meta_charset << "\n";
    }
}

void HtmlExporter::writePageBegin(std::ostream& os, CEDPage&) {
    writeDoctype(os);
    writeStartTag(os, "html", "\n");
    writeStartTag(os, "head", "\n");
    writeTitle(os);
    writeMeta(os);
    writeCloseTag(os, "head", "\n");
    writeStartTag(os, "body", "\n");

    if (os.fail())
        throw Exception("HtmlExporter failed");
}

void HtmlExporter::writePageEnd(std::ostream& os, CEDPage&) {
    writeCloseTag(os, "body", "\n");
    writeCloseTag(os, "html", "\n");
}

void HtmlExporter::writeParagraphBegin(std::ostream& os, CEDParagraph& par) {
    XmlTag p("p");
    switch (par.align()) {
    case ALIGN_CENTER:
        p["align"] = "center";
        break;
    case (ALIGN_JUSTIFY):
        p["align"] = "justify";
        break;
    case ALIGN_RIGHT:
        p["align"] = "right";
    default:
        // "left" by default
        break;
    }

    p.writeBegin(os);
    TextExporter::writeParagraphBegin(os, par);

    prev_char_style_ = 0;
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph&) {
    // writes closing tags to line buffer
    writeFontStyleEnd(prev_char_style_);
    // write and flush line buffer to output stream
    writeLineBufferRaw(os);
    writeCloseTag(os, "p", "\n");
}

void HtmlExporter::writePicture(std::ostream&, CEDPicture& picture) {
    try {
        savePicture(picture);

        XmlTag img("img");
        img["src"] = escapeHtmlSpecialChars(makePicturePathRelative(picture));
        img["alt"] = "";
        img["height"] = toString(picture.height());
        img["width"] = toString(picture.width());
        lineBuffer() << img << "\n";

    } catch (Exception& e) {
        Debug() << "[HtmlExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void HtmlExporter::writeSectionBegin(std::ostream& os, CEDSection&) {
    Attributes attrs;
    attrs["id"] = "section_" + toString(numSections());
    writeStartTag(os, "div", attrs, "\n");
}

void HtmlExporter::writeSectionEnd(std::ostream& os, CEDSection&) {
    writeCloseTag(os, "div", "\n");
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << XmlTag("title", "Cuneiform output") << "\n";
}

}
