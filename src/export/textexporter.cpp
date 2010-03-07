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
    GenericExporter(page, opts) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
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
    replaceAll(res, "--", "\u2013");
    return res;
}

void TextExporter::writeBOM(std::ostream& os) {
    os << "\xEF\xBB\xBF";
}

void TextExporter::writeCharacter(std::ostream& /*os*/, CEDChar * chr) {
    GenericExporter::writeCharacter(line_buffer_, chr);
}

void TextExporter::writeLineBreak(std::ostream& os, CEDLine * line) {
    if (isLineBreak(line))
        os << "\n";
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
}

void TextExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    writeLineBuffer(os, line);
    writeLineBreak(os, line);
}

void TextExporter::writePageEnd(std::ostream& os, CEDPage*) {
    os << std::endl;
}

void TextExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    os << "\n";
}

void TextExporter::writePicture(std::ostream& os, CEDChar * /*picture*/) {
    os << "[picture]\n";
}

}
