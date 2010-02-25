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
#include "ced/cedint.h"
#include "rout_own.h"
#include "imageexporterfactory.h"

namespace CIF
{

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), lines_left_(0), current_font_style_(0) {

    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);
}

HtmlExporter::~HtmlExporter() {
}

void HtmlExporter::writeFontStyleBegin(std::ostream& os, long newStyle, int style) {
    if (newStyle & style && !(current_font_style_ & style)) {
        font_styles_.push_back(style);
        os << fontStyleBegin(style);
    }
}

void HtmlExporter::writeFontStyleClose(std::ostream& os, long newStyle, int style) {
    if (!(newStyle & style) && (current_font_style_ & style)) {
        if (font_styles_.empty())
            return;

        int tag_pos = font_styles_.size() - 1;
        int tag_found = -1;

        while (tag_pos >= 0) {
            os << fontStyleEnd(font_styles_[tag_pos]);
            if (font_styles_[tag_pos] == style) {
                tag_found = tag_pos;
                break;
            }
            tag_pos--;
        }

        tag_pos++;
        while (tag_pos < font_styles_.size()) {
            os << fontStyleBegin(font_styles_[tag_pos]);
            tag_pos++;
        }

        if (tag_found >= 0)
            font_styles_.erase(font_styles_.begin() + tag_found);
    }
}

std::string HtmlExporter::escapeHtmlSpecialChar(uchar code) {
    switch (code) {
    case '>':
        return "&gt;";
    case '<':
        return "&lt;";
    case '&':
        return "&amp;";
    case '"':
        return "&quot;";
    default:
        return std::string(1, code);
    }
}

std::string HtmlExporter::fontStyleBegin(int style) {
    switch (style) {
    case FONT_BOLD:
        return "<b>";
    case FONT_ITALIC:
        return "<i>";
    case FONT_UNDERLINE:
        return "<u>";
    case FONT_SUB:
        return "<sub>";
    case FONT_SUPER:
        return "<sup>";
    default:
        return "";
    }
}

std::string HtmlExporter::fontStyleEnd(int style) {
    switch (style) {
    case FONT_BOLD:
        return "</b>";
    case FONT_ITALIC:
        return "</i>";
    case FONT_UNDERLINE:
        return "</u>";
    case FONT_SUB:
        return "</sub>";
    case FONT_SUPER:
        return "</sup>";
    default:
        return "";
    }
}

void HtmlExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);

    writeFontStyle(os, chr->fontAttribs);

    if (isCharsetConversionNeeded())
        os << converter_.convert(escapeHtmlSpecialChar(chr->alternatives->alternative));
    else
        os << escapeHtmlSpecialChar(chr->alternatives->alternative);
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << "<!DOCTYPE html PUBLIC "
        "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
        "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
}

void HtmlExporter::writeFontStyle(std::ostream& os, long newStyle) {
    if (current_font_style_ == newStyle)
        return;

    if (formatOptions().isBoldUsed())
        writeFontStyleClose(os, newStyle, FONT_BOLD);

    if (formatOptions().isItalicUsed())
        writeFontStyleClose(os, newStyle, FONT_ITALIC);

    writeFontStyleClose(os, newStyle, FONT_UNDERLINE);

    if (formatOptions().isFontSizeUsed()) {
        writeFontStyleClose(os, newStyle, FONT_SUB);
        writeFontStyleClose(os, newStyle, FONT_SUPER);

        writeFontStyleBegin(os, newStyle, FONT_SUPER);
        writeFontStyleBegin(os, newStyle, FONT_SUB);
    }

    writeFontStyleBegin(os, newStyle, FONT_UNDERLINE);

    if (formatOptions().isItalicUsed())
        writeFontStyleBegin(os, newStyle, FONT_ITALIC);

    if (formatOptions().isBoldUsed())
        writeFontStyleBegin(os, newStyle, FONT_BOLD);

    // save font style
    current_font_style_ = newStyle;
}

void HtmlExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    lines_left_--;
    if (lines_left_ < 1)
        return;

    if (formatOptions().preserveLineBreaks() || line->hardBreak)
        os << "<br/>";
    os << "\n";
}

void HtmlExporter::writeMeta(std::ostream& os) {
    Attributes attrs;
    attrs["name"] = "Generator";
    attrs["content"] = "Cuneiform";
    writeSingleTag(os, "meta", attrs);
    os << "\n";

    if (isCharsetConversionNeeded()) {
        attrs["name"] = "Content-Type";
        attrs["content"] = "text/html;charset=" + outputEncoding();
        writeSingleTag(os, "meta", attrs);
        os << "\n";
    }
}

void HtmlExporter::writePageBegin(std::ostream& os) {
    writeDoctype(os);
    os << "<html>\n<head>\n";
    writeTitle(os);
    writeMeta(os);
    os << "</head>\n<body>\n";
    writeFontStyle(os, 0);

    if (os.fail())
        throw Exception("HtmlExporter failed");
}

void HtmlExporter::writePageEnd(std::ostream& os) {
    writeFontStyle(os, 0);
    os << "</body>\n</html>\n";
}

void HtmlExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    assert(par);

    Attributes attrs;
    switch (par->alignment & ALIGN_MASK) {
    case ALIGN_CENTER:
        attrs["align"] = "center";
        break;
    case (ALIGN_LEFT | ALIGN_RIGHT):
        attrs["align"] = "justify";
        break;
    case ALIGN_RIGHT:
        attrs["align"] = "right";
    default:
        // "left" by default
        break;
    }

    writeFontStyle(os, 0);
    writeStartTag(os, "p", attrs);

    lines_left_ = par->GetCountLine();
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    writeFontStyle(os, 0);
    os << "</p>\n";
}

void HtmlExporter::writePicture(std::ostream& os, CEDChar * picture) {
    std::string path = savePicture(picture);
    Attributes attrs;
    attrs["src"] = path;
    attrs["alt"] = "";
    attrs["height"] = toString(last_picture_size_.height());
    attrs["width"] = toString(last_picture_size_.width());
    writeSingleTag(os, "img", attrs);
}

void HtmlExporter::writeTableBegin(std::ostream& os, CEDParagraph * /*table*/) {
    writeFontStyle(os, 0);
    os << "<table>";
}

void HtmlExporter::writeTableEnd(std::ostream& os, CEDParagraph * /*table*/) {
    os << "</table>";
}

void HtmlExporter::writeAttributes(std::ostream& os, const Attributes& attrs) {
    for (Attributes::const_iterator it = attrs.begin(), end = attrs.end(); it != end; ++it)
        os << " " << it->first << "=\"" << it->second << "\"";
}

void HtmlExporter::writeSingleTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs) {
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << "/>";
}

void HtmlExporter::writeStartTag(std::ostream& os, const std::string& tagName) {
    os << "<" << tagName << ">";
}

void HtmlExporter::writeStartTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs) {
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << ">";
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << "<title>Cuneiform output</title>\n";
}

}
