/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include "textexporter.h"
#include "ced/ced.h"
#include "ced/cedint.h"
#include "ced/cedline.h"
#include "common/helper.h"

#include <cstdlib>
#include <cstring>

namespace CIF
{

inline void removeHyphens(std::string& line) {
    if (line.empty())
        return;

    size_t len = line.size();
    if (len > 1 && line[len - 1] == '-')
        line.erase(len - 1, 1);
}

TextExporter::TextExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), line_hard_break_flag_(false) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

void TextExporter::clearLineBuffer() {
    assert(line_buffer_.rdbuf());
    line_buffer_.rdbuf()->str("");
}

std::string TextExporter::escapeXmlSpecialChar(uchar code) {
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

void TextExporter::exportChar(CEDChar * chr) {
    if (CED_IsPicture(chr))
        return GenericExporter::exportChar(chr);

    std::ostream * old_stream = outputStream();
    setOutputStream(&line_buffer_);
    GenericExporter::exportChar(chr);
    setOutputStream(old_stream);
}

void TextExporter::exportTo(std::ostream& os) {
    writeBOM(os);
    GenericExporter::exportTo(os);
}

bool TextExporter::isLineBreak() const {
    return formatOptions().preserveLineBreaks() || line_hard_break_flag_;
}

std::ostringstream& TextExporter::lineBuffer() {
    return line_buffer_;
}

std::string TextExporter::lineBufferString() {
    std::string res(line_buffer_.str());
    replaceAll(res, "--", "\u2013");
    return res;
}

void TextExporter::writeBOM(std::ostream& os) {
#ifdef __APPLE__
    os << "\xEF\xBB\xBF";
#endif
}

void TextExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);
    os << std::string(1, chr->alternatives->alternative);
}

void TextExporter::writeLineBegin(std::ostream& /*os*/, CEDLine * line) {
    line_hard_break_flag_ = line->hardBreak();
}

void TextExporter::writeLineBreak(std::ostream& os) {
    if (isLineBreak())
        os << "\n";
}

void TextExporter::writeLineEnd(std::ostream& os, CEDLine * /*line*/) {
    std::string line = lineBufferString();

    if (!isLineBreak() && !formatOptions().preserveLineHyphens())
        removeHyphens(line);

    if (isCharsetConversionNeeded())
        os << converter_.convert(line);
    else
        os << line;

    writeLineBreak(os);
    clearLineBuffer();
}

void TextExporter::writePageBegin(std::ostream& os) {
    os << "\n";
}

void TextExporter::writePageEnd(std::ostream& os) {
    os << std::endl;
}

void TextExporter::writeParagraphBegin(std::ostream& /*os*/, CEDParagraph * /*par*/) {
}

void TextExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    os << "\n";
}

void TextExporter::writePicture(std::ostream& os, CEDChar * /*picture*/) {
    os << "[picture]\n";
}

void TextExporter::writeTableBegin(std::ostream& os, CEDParagraph * /*table*/) {
    os << "[table]\n";
}

void TextExporter::writeTableEnd(std::ostream& /*os*/, CEDParagraph * /*table*/) {
}

}
