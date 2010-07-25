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
    TextExporter(page, opts), line_break_(false) {
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

void XmlExporter::writeCloseTag(const std::string& tagName, const std::string& newline) {
    if (tagName.empty())
        return;

    outputStream() << "</" << tagName << ">" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeXmlDeclaration(const std::string& encoding) {
    outputStream() << "<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>\n";
}

void XmlExporter::writeSingleTag(const std::string& tagName, const Attributes& attrs,
        const std::string& newline) {
    if (tagName.empty())
        return;

    outputStream() << "<" << tagName;
    writeAttributes(attrs);
    outputStream() << "/>" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeSingleTag(const std::string& tagName, const std::string& newline) {
    if (tagName.empty())
        return;

    outputStream() << "<" << tagName << "/>" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeAttributes(const Attributes& attrs) {
    for (Attributes::const_iterator it = attrs.begin(), end = attrs.end(); it != end; ++it)
        outputStream() << " " << it->first << "=\"" << it->second << "\"";
}

void XmlExporter::writeStartTag(const std::string& tagName, const std::string& newline) {
    if (tagName.empty())
        return;

    outputStream() << "<" << tagName << ">" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeStartTag(const std::string& tagName, const Attributes& attrs,
        const std::string& newline) {
    if (tagName.empty())
        return;

    outputStream() << "<" << tagName;
    writeAttributes(attrs);
    outputStream() << ">" << newline;
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeTag(const std::string& tagName, const std::string& tagText,
        const Attributes& attrs, const std::string& newline) {
    if (tagName.empty())
        return;

    writeStartTag(tagName, attrs);
    outputStream() << escapeHtmlSpecialChars(tagText);
    writeCloseTag(tagName, newline);
    line_break_ = newline.empty() ? false : true;
}

void XmlExporter::writeCharacter(CEDChar& chr) {
    assert(chr.hasAlternatives());
    if (elements_left_in_line_-- == 1 && remove_last_line_hyphen_)
        return;

    lineBuffer() << escapeSpecialChar(chr.alternativeAt(0).getChar());
}

}
