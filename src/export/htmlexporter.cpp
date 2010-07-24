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

#include "cssexporter.h"
#include "htmlexporter.h"
#include "imageexporterfactory.h"
#include "rout_own.h"
#include "xmltag.h"

#include "common/helper.h"
#include "common/debug.h"
#include "common/font.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "config.h" // for CF_VERSION
#ifndef CF_VERSION
#define CF_VERSION "unknown"
#endif

namespace CIF
{

const std::string HTML_ALTERNATIVE_STYLE_CLASS("has_alternative");
const std::string HTML_ALTERNATIVE_STYLE_CONTENT("color: #FF3333; padding: 0 2px;\n");

const std::string HTML_DOCTYPE("<!DOCTYPE html PUBLIC "
    "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    XmlExporter(page, opts), prev_char_style_hash_(0), prev_char_font_style_(0), char_span_opened_(
            false), style_exporter_(new CssExporter(page, opts)) {

    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);

    if (formatOptions().showAlternatives())
        style_exporter_->addCssStyle(HTML_ALTERNATIVE_STYLE_CLASS, HTML_ALTERNATIVE_STYLE_CONTENT);
}

HtmlExporter::~HtmlExporter() {
    delete style_exporter_;
}

void HtmlExporter::writeAlternativesBegin(const CEDChar& chr) {
    if (chr.alternativeCount() > 1) {
        lineBuffer() << "<span title=\"Alternatives:";
        for (size_t i = 1; i < chr.alternativeCount(); i++)
            lineBuffer() << " " << escapeSpecialChar(chr.alternativeAt(i).getChar());

        lineBuffer() << "\" class=\"" << HTML_ALTERNATIVE_STYLE_CLASS << "\">";
    }
}

void HtmlExporter::writeAlternativesEnd(const CEDChar& chr) {
    if (chr.alternativeCount() > 1)
        lineBuffer() << "</span>";
}

void HtmlExporter::writeCharacterBegin(CEDChar& chr) {
    size_t chr_hash = style_exporter_->hash(chr);
    if (style_exporter_->hasHash(chr_hash)) {
        if (prev_char_style_hash_ != chr_hash) {
            if (char_span_opened_) {
                lineBuffer() << "</span>";
                char_span_opened_ = false;
            }

            XmlTag span("span");
            span["class"] = style_exporter_->styleByHash(chr_hash);
            span.writeBegin(lineBuffer());
            char_span_opened_ = true;

            prev_char_style_hash_ = chr_hash;
        }
    }

    if (prev_char_font_style_ != chr.fontStyle()) {
        writeFontStyleEnd(prev_char_font_style_);
        writeFontStyleBegin(chr.fontStyle());
        prev_char_font_style_ = chr.fontStyle();
    }

    if (formatOptions().showAlternatives())
        writeAlternativesBegin(chr);
}

void HtmlExporter::writeCharacterEnd(CEDChar& chr) {
    if (formatOptions().showAlternatives())
        writeAlternativesEnd(chr);
}

void HtmlExporter::writeCssStyle() {
    writeStartTag("style", "\n");
    outputStream() << "<!--\n";
    style_exporter_->exportTo(outputStream());
    outputStream() << "-->\n";
    writeCloseTag("style", "\n");
}

void HtmlExporter::writeDoctype() {
    outputStream() << HTML_DOCTYPE;
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

void HtmlExporter::writeFrameBegin(CEDFrame&) {
    outputStream() << "<div class=\"frame\">";
}

void HtmlExporter::writeFrameEnd(CEDFrame&) {
    outputStream() << "</div>\n";
}

void HtmlExporter::writeLineBreak() {
    outputStream() << XmlTag("br") << "\n";
}

void HtmlExporter::writeMeta() {
    XmlTag meta_generator("meta");
    meta_generator["name"] = "Generator";
    meta_generator["content"] = "cuneiform-linux-" CF_VERSION;
    outputStream() << meta_generator << "\n";

    if (isCharsetConversion()) {
        XmlTag meta_charset("meta");
        meta_charset["name"] = "Content-Type";
        meta_charset["content"] = "text/html;charset=" + outputEncoding();
        outputStream() << meta_charset << "\n";
    }
}

void HtmlExporter::writePageBegin(CEDPage&) {
    writeDoctype();
    writeStartTag("html", "\n");
    writeStartTag("head", "\n");
    writeTitle();
    writeMeta();
    writeCssStyle();
    writeCloseTag("head", "\n");
    writeStartTag("body", "\n");
}

void HtmlExporter::writePageEnd(CEDPage&) {
    writeCloseTag("body", "\n");
    writeCloseTag("html", "\n");
}

void HtmlExporter::writeParagraphBegin(CEDParagraph& par) {
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

    std::string par_style = style_exporter_->styleByElement(par);
    if (!par_style.empty())
        p["class"] = escapeHtmlSpecialChars(par_style);

    p.writeBegin(outputStream());
    TextExporter::writeParagraphBegin(par);

    prev_char_font_style_ = 0;
    prev_char_style_hash_ = 0;
}

void HtmlExporter::writeParagraphEnd(CEDParagraph&) {
    // writes closing tags to line buffer
    writeFontStyleEnd(prev_char_font_style_);
    if (char_span_opened_) {
        lineBuffer() << "</span>";
        char_span_opened_ = false;
    }
    // write and flush line buffer to output stream
    writeLineBufferRaw();
    writeCloseTag("p", "\n");
}

void HtmlExporter::writePicture(CEDPicture& picture) {
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

void HtmlExporter::writeSectionBegin(CEDSection&) {
    Attributes attrs;
    attrs["id"] = "section_" + toString(numSections());
    writeStartTag("div", attrs, "\n");
}

void HtmlExporter::writeSectionEnd(CEDSection&) {
    writeCloseTag("div", "\n");
}

void HtmlExporter::writeTitle() {
    outputStream() << XmlTag("title", "Cuneiform output") << "\n";
}

}
