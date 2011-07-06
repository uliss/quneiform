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

#include "common/helper.h"
#include "common/debug.h"
#include "common/font.h"
#include "common/fontstyle.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "config-version.h" // for CF_VERSION

namespace cf
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

void HtmlExporter::changeCharacterFontStyle(int new_style) {
    closeCharacterFontStyle();
    writeFontStyleBegin(new_style);
    prev_char_font_style_ = new_style;
}

void HtmlExporter::changeCharacterStyleSpan(size_t new_hash) {
    // if previous char style was opened close it
    closeCharacterStyleSpan();
    openCharacterStyleSpan(new_hash);
    prev_char_style_hash_ = new_hash;
}

void HtmlExporter::closeCharacterFontStyle() {
    writeFontStyleEnd(prev_char_font_style_);
}

void HtmlExporter::closeCharacterStyleSpan() {
    if (!char_span_opened_ || !formatOptions().isStylesUsed())
        return;

    writeCloseTag("span");
    char_span_opened_ = false;
}

void HtmlExporter::openCharacterStyleSpan(size_t hash) {
    if(!formatOptions().isStylesUsed())
        return;

    Attributes attrs;
    attrs["class"] = style_exporter_->styleByHash(hash);
    writeStartTag("span", attrs);
    char_span_opened_ = true;
}

void HtmlExporter::openParagraphTag(const CEDParagraph& par) {
    Attributes attrs;
    switch (par.align()) {
    case ALIGN_CENTER:
        attrs["align"] = "center";
        break;
    case (ALIGN_JUSTIFY):
        attrs["align"] = "justify";
        break;
    case ALIGN_RIGHT:
        attrs["align"] = "right";
    default:
        // "left" by default
        break;
    }

    std::string par_style = style_exporter_->styleByElement(par);
    if (!par_style.empty())
        attrs["class"] = escapeHtmlSpecialChars(par_style);

    writeStartTag("p", attrs);
}

void HtmlExporter::resetCharacterFontStyle() {
    prev_char_font_style_ = 0;
}

void HtmlExporter::resetCharacterStyleSpan() {
    prev_char_style_hash_ = 0;
}

void HtmlExporter::writeAlternativesBegin(const CEDChar& chr) {
    if (chr.alternativeCount() > 1) {
        Attributes attrs;
        attrs["class"] = HTML_ALTERNATIVE_STYLE_CLASS;

        std::string alternatives("Alternatives:");
        for (size_t i = 1; i < chr.alternativeCount(); i++)
            alternatives.append(" " + escapeSpecialChar(chr.alternativeAt(i).getChar()));

        attrs["title"] = alternatives;

        writeStartTag("span", attrs);
    }
}

void HtmlExporter::writeAlternativesEnd(const CEDChar& chr) {
    if (chr.alternativeCount() > 1)
        writeCloseTag("span");
}

void HtmlExporter::writeCharacterBegin(CEDChar& chr) {
    size_t chr_hash = style_exporter_->hash(chr);
    if (style_exporter_->hasHash(chr_hash)) {
        // character style changed
        if (prev_char_style_hash_ != chr_hash)
            changeCharacterStyleSpan(chr_hash);
    }

    // font style changed
    if (prev_char_font_style_ != chr.fontStyle())
        changeCharacterFontStyle(chr.fontStyle());

    if (formatOptions().showAlternatives())
        writeAlternativesBegin(chr);
}

void HtmlExporter::writeCharacterEnd(CEDChar& chr) {
    if (formatOptions().showAlternatives())
        writeAlternativesEnd(chr);
}

void HtmlExporter::writeCssStyle() {
    if (!formatOptions().isStylesUsed())
        return;

    writeStartTag("style", "\n");
    buffer() << "<!--\n";
    style_exporter_->exportTo(buffer());
    buffer() << "-->\n";
    writeCloseTag("style", "\n");
}

void HtmlExporter::writeDoctype() {
    buffer() << HTML_DOCTYPE;
}

void HtmlExporter::writeFontStyleBegin(int style) {
    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        writeStartTag("b");

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        writeStartTag("i");

    if (formatOptions().isUnderlinedUsed() && style & FONT_UNDERLINE)
        writeStartTag("u");

    if (style & FONT_SUB)
        writeStartTag("sub");

    if (style & FONT_SUPER)
        writeStartTag("sup");
}

void HtmlExporter::writeFontStyleEnd(int style) {
    if (style & FONT_SUPER)
        writeCloseTag("sup");

    if (style & FONT_SUB)
        writeCloseTag("sub");

    if (formatOptions().isUnderlinedUsed() && style & FONT_UNDERLINE)
        writeCloseTag("u");

    if (formatOptions().isItalicUsed() && (style & FONT_ITALIC))
        writeCloseTag("i");

    if (formatOptions().isBoldUsed() && (style & FONT_BOLD))
        writeCloseTag("b");
}

void HtmlExporter::writeFrameBegin(CEDFrame&) {
    Attributes attrs;
    attrs["class"] = "frame";
    writeStartTag("div", attrs);
}

void HtmlExporter::writeFrameEnd(CEDFrame&) {
    writeCloseTag("div", "\n");
}

void HtmlExporter::writeLineBreak() {
    writeSingleTag("br", "\n");
}

void HtmlExporter::writeMeta() {
    if(!formatOptions().isTestOutput() && formatOptions().writeMetaGenerator()) {
        Attributes meta_generator;
        meta_generator["name"] = "Generator";
        meta_generator["content"] = "cuneiform-linux-" CF_VERSION;
        writeSingleTag("meta", meta_generator, "\n");
    }

    if (isCharsetConversion()) {
        Attributes meta_charset;
        meta_charset["name"] = "Content-Type";
        meta_charset["content"] = "text/html;charset=" + outputEncoding();
        writeSingleTag("meta", meta_charset, "\n");
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
    flushBuffer();
}

void HtmlExporter::writeParagraphBegin(CEDParagraph& par) {
    openParagraphTag(par);
    TextExporter::writeParagraphBegin(par);

    resetCharacterFontStyle();
    resetCharacterStyleSpan();
}

void HtmlExporter::writeParagraphEnd(CEDParagraph&) {
    closeCharacterFontStyle();
    closeCharacterStyleSpan();
    writeCloseTag("p", "\n");
}

void HtmlExporter::writePicture(CEDPicture& picture) {
    try {
        savePicture(picture);
        writePictureTag(picture);
    } catch (Exception& e) {
        Debug() << "[HtmlExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void HtmlExporter::writePictureTag(const CEDPicture& picture) {
    Attributes attrs;
    attrs["src"] = escapeHtmlSpecialChars(makePicturePathRelative(picture));
    attrs["alt"] = "";
    attrs["height"] = toString(picture.height());
    attrs["width"] = toString(picture.width());
    writeSingleTag("img", attrs, "\n");
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
    writeTag("title", "Cuneiform output", Attributes(), "\n");
}

}
