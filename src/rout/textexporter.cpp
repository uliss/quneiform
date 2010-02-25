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
#include "common/helper.h"

#include <cstdlib>
#include <cstring>

namespace CIF
{

TextExporter::TextExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), line_hard_break_flag_(false) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

std::string TextExporter::convertLineBuffer() {
    preprocessLine(line_buffer_);
    return isCharsetConversionNeeded() ? converter_.convert(line_buffer_) : line_buffer_;
}

void TextExporter::exportTo(std::ostream& os) {
    writeBOM(os);
    GenericExporter::exportTo(os);
}

void TextExporter::preprocessLine(std::string& line) {
    replaceAll(line, "--", "\u2013");
}

void TextExporter::removeHyphens() {
    if (line_buffer_.empty())
        return;

    size_t len = line_buffer_.size();
    if (len > 1 and line_buffer_[len - 1] == '-')
        line_buffer_.erase(len - 1, 1);
}

void TextExporter::writeBOM(std::ostream& os) {
#ifdef __APPLE__
    os << "\xEF\xBB\xBF";
#endif
}

void TextExporter::writeCharacter(std::ostream& /*os*/, CEDChar * chr) {
    assert(chr && chr->alternatives);
    line_buffer_.append(1, chr->alternatives->alternative);
}

void TextExporter::writeLineBegin(std::ostream& /*os*/, CEDLine * line) {
    line_hard_break_flag_ = line->hardBreak ? true : false;
    line_buffer_.clear();
}

void TextExporter::writeLineEnd(std::ostream& os, CEDLine * /*line*/) {
    if (formatOptions().preserveLineBreaks() or line_hard_break_flag_) {
        os << convertLineBuffer() << "\n";
    } else {
        if (!formatOptions().preserveLineHyphens())
            removeHyphens();
        os << convertLineBuffer();
    }
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
