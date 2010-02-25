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

#include <cstdlib>
#include <cstring>

namespace CIF
{

TextExporter::TextExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts) {
    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(true);
}

void TextExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);
    if (isCharsetConversionNeeded())
        os << converter_.convert(std::string(1, chr->alternatives->alternative));
    else
        os << chr->alternatives->alternative;
}

void TextExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    if (formatOptions().preserveLineBreaks() or line->hardBreak)
        os << "\n";
}

void TextExporter::writePageBegin(std::ostream& os) {
    os << "\n";
}

void TextExporter::writePageEnd(std::ostream& os) {
    os << std::endl;
}

void TextExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
}

void TextExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * par) {
    os << "\n";
}

void TextExporter::writePicture(std::ostream& os, CEDChar * picture) {
    os << "[picture]\n";
}

void TextExporter::writeTableBegin(std::ostream& os, CEDParagraph * table) {
    os << "[table]\n";
}

void TextExporter::writeTableEnd(std::ostream& os, CEDParagraph * table) {
}

}
