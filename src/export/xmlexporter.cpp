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

#include "xmlexporter.h"
#include "common/helper.h"
#include "ced/cedchar.h"

namespace CIF
{

const int INDENT_STEP = 2;
const char INDENT_CHAR = ' ';

inline std::string makeIndent(int level) {
    if (level < 1)
        return "";
    return std::string(level * INDENT_STEP, INDENT_CHAR);
}

XmlExporter::XmlExporter(CEDPage * page, const FormatOptions& opts) :
    TextExporter(page, opts), indent_level_(0), use_indent_(false), line_break_(false) {
}

void XmlExporter::appendTo(const std::string& filename) {
    Exporter::appendTo(filename);
}

std::string XmlExporter::escapeSpecialChar(uchar code) {
    switch (code) {
    case '>':
        return "&gt;";
    case '<':
        return "&lt;";
    case '&':
        return "&amp;";
    case '"':
        return "&quot;";
    case '\'':
        return "&apos;";
    default:
        return std::string(1, code);
    }
}

std::string XmlExporter::fontStyleTag(int) const {
    return std::string();
}

void XmlExporter::writeCloseTag(std::ostream& os, const std::string& tagName,
        const std::string& newline) {
    if (tagName.empty())
        return;

    indent_level_--;
    writeIndent(os);
    os << "</" << tagName << ">" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeFontStyleBegin(std::ostream& /*os*/, int style) {
    writeStartTag(lineBuffer(), fontStyleTag(style));
}

void XmlExporter::writeFontStyleEnd(std::ostream& /*os*/, int style) {
    writeCloseTag(lineBuffer(), fontStyleTag(style));
}

void XmlExporter::writeXmlDeclaration(std::ostream& os, const std::string& encoding) {
    os << "<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>\n";
}

void XmlExporter::writeIndent(std::ostream& os) {
    if (use_indent_ && line_break_)
        os << makeIndent(indent_level_);
}

void XmlExporter::writeSingleTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs, const std::string& newline) {
    if (tagName.empty())
        return;

    writeIndent(os);
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << "/>" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeAttributes(std::ostream& os, const Attributes& attrs) {
    for (Attributes::const_iterator it = attrs.begin(), end = attrs.end(); it != end; ++it)
        os << " " << it->first << "=\"" << it->second << "\"";
}

void XmlExporter::writeStartTag(std::ostream& os, const std::string& tagName,
        const std::string& newline) {
    if (tagName.empty())
        return;

    writeIndent(os);
    os << "<" << tagName << ">" << newline;
    indent_level_++;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeStartTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs, const std::string& newline) {
    if (tagName.empty())
        return;

    writeIndent(os);
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << ">" << newline;
    indent_level_++;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeTag(std::ostream& os, const std::string& tagName,
        const std::string& tagText, const Attributes& attrs, const std::string& newline) {
    if (tagName.empty())
        return;

    writeStartTag(os, tagName, attrs);
    os << escapeHtmlSpecialChars(tagText);
    writeCloseTag(os, tagName, newline);
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeCharacter(std::ostream& /*os*/, CEDChar& chr) {
    assert(chr.hasAlternatives());
    lineBuffer() << escapeSpecialChar(chr.alternativeAt(0).getChar());
}

void XmlExporter::useIndents(bool value) {
    use_indent_ = value;
}

}
