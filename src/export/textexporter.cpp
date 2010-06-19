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

#include <cstdlib>
#include <cstring>
#include <fstream>

#include "textexporter.h"
#include "ced/ced.h"
#include "ced/cedint.h"
#include "ced/cedline.h"
#include "common/helper.h"

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
    GenericExporter(page, opts), lines_left_in_paragraph_(0) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

void TextExporter::appendTo(const std::string& filename) {
    std::ofstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    if (!f)
        throw Exception("[TextExporter::appendTo] append failed to: " + filename);
    setOutputFilename(filename);
    doExport(f);
}

void TextExporter::bufferPreprocess(std::string& buffer) {
    // replaces -- to Unicode mdash character
    replaceAll(buffer, "--", "\u2013");
}

void TextExporter::clearLineBuffer() {
    assert(line_buffer_.rdbuf());
    line_buffer_.rdbuf()->str("");
}

void TextExporter::exportTo(std::ostream& os) {
#ifdef __APPLE__
    writeBOM(os);
#endif
    GenericExporter::exportTo(os);
}

std::ostringstream& TextExporter::lineBuffer() {
    return line_buffer_;
}

std::string TextExporter::lineBufferPrepared() {
    std::string res(line_buffer_.str());
    bufferPreprocess(res);
    return res;
}

int TextExporter::lineLeftInParagraph() const {
    return lines_left_in_paragraph_;
}

void TextExporter::writeBOM(std::ostream& os) {
    os << "\xEF\xBB\xBF";
}

void TextExporter::writeCharacter(std::ostream& /*os*/, CEDChar * chr) {
    GenericExporter::writeCharacter(line_buffer_, chr);
}

void TextExporter::writeLineBreak(std::ostream& os) {
    os << '\n';
}

void TextExporter::writeLineBuffer(std::ostream& os, CEDLine * line) {
    std::string output_line = lineBufferPrepared();

    if (isRemoveHyphens(line))
        removeHyphens(output_line);

    if (isCharsetConversion())
        os << converter_.convert(output_line);
    else
        os << output_line;

    clearLineBuffer();

    if (isLineBreak(line)) {
        writeLineBreak(os);
    } else {
        // if no end hyphen
        // insert space to separate words on different lines
        if (!output_line.empty() && output_line.rfind('-') != 0 && lines_left_in_paragraph_ > 1)
            os << ' ';
    }
}

void TextExporter::writeLineBufferRaw(std::ostream& os) {
    os << lineBuffer().str();
    clearLineBuffer();
}

void TextExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    writeLineBuffer(os, line);
    lines_left_in_paragraph_--;
}

void TextExporter::writePageEnd(std::ostream& os, CEDPage*) {
    os << std::endl;
}

void TextExporter::writeParagraphBegin(std::ostream&, CEDParagraph * par) {
    lines_left_in_paragraph_ = par->lineCount();
}

void TextExporter::writeParagraphEnd(std::ostream& os, CEDParagraph*) {
    os << "\n";
}

void TextExporter::writePicture(std::ostream& os, CEDChar * /*picture*/) {
    os << "[picture]\n";
}

}
