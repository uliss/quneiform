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
#include "common/iconv_local.h"

namespace CIF
{

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts) {
    converter_ = new Iconv;
    converter_->open("cp1251", "utf-8");
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
        return "&quot";
    default:
        return std::string(1, code);
    }
}

void HtmlExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);
    os << converter_->convert(escapeHtmlSpecialChar(chr->alternatives->alternative));
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" "
        "\"http://www.w3.org/TR/html4/loose.dtd\">\n";
}

void HtmlExporter::writeMeta(std::ostream& os) {
    os << "  <meta name=\"Generator\" content=\"Cuneiform\"/>\n";
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
    const int ALIGN_MASK = (TP_LEFT_ALLIGN || TP_RIGHT_ALLIGN || TP_CENTER);
    switch (par->alignment & ALIGN_MASK) {
    case TP_CENTER:
        os << " align=\"center\"";
        break;
    case (TP_LEFT_ALLIGN || TP_RIGHT_ALLIGN):
        os << " align=\"justify\"";
        break;
    default:
        // "left" by default
        break;
    }
    os << ">";
    par_pos_ = os.tellp();
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * par) {
    os << "</p>\n";
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << "  <title>Cuneiform output</title>\n";
}

}
