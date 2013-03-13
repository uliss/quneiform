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
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedpicture.h"
#include "common/helper.h"
#include "common/log.h"

namespace cf
{

TextExporter::TextExporter(CEDPagePtr page, const FormatOptions& opts) :
    GenericExporter(page, opts),
    lines_left_in_paragraph_(0),
    elements_left_in_line_(0),
    remove_last_line_hyphen_(false)
{

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);
}

void TextExporter::appendTo(const std::string& filename) {
    std::ofstream f(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    if (!f)
        throw Exception("[TextExporter::appendTo] append failed to: " + filename);
    setOutputFilename(filename);
    doExport(f);
}

void TextExporter::doExport(std::ostream& os) {
    if(formatOptions().writeBom())
        writeBOM(os);

    GenericExporter::doExport(os);
}

void TextExporter::flushBuffer() {
    if (!outputStream()) {
        cfError(MODULE_EXPORT) << "[TextExporter::flushBuffer] pointer to output stream is NULL";
        return;
    }

    if (isCharsetConversion())
        flushBufferConverted();
    else
        flushBufferRaw();
}

void TextExporter::flushBufferConverted() {
    *outputStream() << converter_.convert(buffer_.str());
    outputStream()->flush();
    buffer_.str("");
}

void TextExporter::flushBufferRaw() {
    *outputStream() << buffer_.str();
    outputStream()->flush();
    buffer_.str("");
}

void TextExporter::writeBOM(std::ostream& os) {
    os << "\xEF\xBB\xBF";
}

void TextExporter::writeCharacter(CEDChar& chr) {
    // skip last hyphen in line
    if (elements_left_in_line_-- == 1 && remove_last_line_hyphen_)
        return;

    buffer() << chr.get();
}

void TextExporter::writeLineBreak() {
    buffer() << '\n';
}

void TextExporter::writeLineBegin(CEDLine& line) {
    elements_left_in_line_ = line.elementCount();
    remove_last_line_hyphen_ = false;

    if (notLastLine() && !isLineBreak(line))
        remove_last_line_hyphen_ = line.endsWithHyphen();
}

void TextExporter::writeLineEnd(CEDLine& line) {
    if (isLineBreak(line) && lines_left_in_paragraph_ > 1) // skip last line break
        writeLineBreak();
    else if (notLastLine() && !remove_last_line_hyphen_)
        buffer() << ' ';

    lines_left_in_paragraph_--;
}

void TextExporter::writePageEnd(CEDPage&) {
    buffer() << '\n';
    flushBuffer();
}

void TextExporter::writeParagraphBegin(CEDParagraph& par) {
    lines_left_in_paragraph_ = par.lineCount();
}

void TextExporter::writeParagraphEnd(CEDParagraph&) {
    buffer() << '\n';
}

void TextExporter::writePicture(CEDPicture& pict) {
    buffer() << "[picture: " << pict.width() << 'x' << pict.height() << "]\n";
}

void TextExporter::writeSectionEnd(CEDSection&) {
    buffer() << '\n';
}

}
