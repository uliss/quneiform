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
#include "htmlexporter.h"
#include "edfile.h"
#include "ced/cedint.h"

#ifdef USE_ICONV
#include "common/iconv_local.h"
#endif

namespace CIF
{

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), converter_(0) {

    setEncodings();

#ifdef USE_ICONV
    if(isCharsetConversionNeeded()) {
        converter_ = new Iconv(inputEncoding(), outputEncoding());
    }
#endif

    setSkipEmptyLines(true);
    setSkipEmptyParagraphhs(true);
}

HtmlExporter::~HtmlExporter() {
#ifdef USE_ICONV
    delete converter_;
#endif
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

void HtmlExporter::setEncodings() {
    switch (formatOptions().language()) {
    case LANGUAGE_RUSSIAN:
    case LANGUAGE_RUS_ENG:
        setInputEncoding("cp1251");
        setOutputEncoding("utf-8");
    default:
        ;
    }
}

void HtmlExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);

#ifdef USE_ICONV
    if(isCharsetConversionNeeded())
    os << converter_->convert(escapeHtmlSpecialChar(chr->alternatives->alternative));
    else
    os << escapeHtmlSpecialChar(chr->alternatives->alternative);
#else
    os << escapeHtmlSpecialChar(chr->alternatives->alternative);
#endif
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << "<!DOCTYPE html PUBLIC "
        "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
        "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
}

void HtmlExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    num_lines_--;
    if (num_lines_ < 1)
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

#ifdef USE_ICONV
    if(isCharsetConversionNeeded()) {
        attrs["name"] = "Content-Type";
        attrs["content"] = "text/html;charset=" + outputEncoding();
        writeSingleTag(os, "meta", attrs);
        os << "\n";
    }
#endif
}

void HtmlExporter::writePageBegin(std::ostream& os) {
    writeDoctype(os);
    os << "<html>\n<head>\n";
    writeTitle(os);
    writeMeta(os);
    os << "</head>\n<body>\n";
}

void HtmlExporter::writePageEnd(std::ostream& os) {
    os << "</body>\n</html>\n";
}

void HtmlExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    assert(par);

    Attributes attrs;
    const int ALIGN_MASK = (TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN | TP_CENTER);
    switch (par->alignment & ALIGN_MASK) {
    case TP_CENTER:
        attrs["align"] = "center";
        break;
    case (TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN):
        attrs["align"] = "justify";
        break;
    default:
        // "left" by default
        break;
    }

    writeStartTag(os, "p", attrs);

    num_lines_ = par->GetCountLine();
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    os << "</p>\n";
}

void HtmlExporter::writePicture(std::ostream& os, CEDChar * picture) {
    std::string path = savePicture(picture);
    Attributes attrs;
    attrs["src"] = path;
    attrs["alt"] = "";
    writeSingleTag(os, "img", attrs);
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
