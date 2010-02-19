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
#ifdef USE_ICONV
    if(isCharsetConversionNeeded()) {
        Charset cset = fromToCharset();
        converter_ = new Iconv;
        converter_->open(cset.first, cset.second);
    }
#endif

    setSkipEmptyLines(true);
    setSkipEmptyParagraphhs(true);
}

HtmlExporter::~HtmlExporter() {
    delete converter_;
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

HtmlExporter::Charset HtmlExporter::fromToCharset() const {
    switch (formatOptions().language()) {
    case LANGUAGE_RUSSIAN:
    case LANGUAGE_RUS_ENG:
        return Charset("cp1251", "utf-8");
    default:
        return Charset();
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
    os << "  <meta name=\"Generator\" content=\"CuneiForm\"/>\n";
    os << "  <meta name=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
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
    os << "<p";
    const int ALIGN_MASK = (TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN | TP_CENTER);
    switch (par->alignment & ALIGN_MASK) {
    case TP_CENTER:
        os << " align=\"center\"";
        break;
    case (TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN):
        os << " align=\"justify\"";
        break;
    default:
        // "left" by default
        break;
    }
    os << ">";

    num_lines_ = par->GetCountLine();
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    os << "</p>\n";
}

void HtmlExporter::writePicture(std::ostream& os, CEDChar * picture) {
    savePicture(picture);
    os << "<img src=\"\" alt=\"\"";
    os << "/>";
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << "  <title>Cuneiform output</title>\n";
}

}
