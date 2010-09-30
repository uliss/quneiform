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

#include <iomanip>
#include <cstdio>
#include "djvutxtexporter.h"
// ced
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"

namespace cf
{

inline bool isSpace(const CEDChar& chr) {
    return chr.boundingRect() == Rect(Point(-1, -1), Point(-1, -1));
}

DjvuTxtExporter::DjvuTxtExporter(CEDPage * page, const FormatOptions& opts) :
    TextExporter(page, opts), indent_level_(0), word_flag_(false) {
    formatOptions().setPreserveLineBreaks(true);
}

void DjvuTxtExporter::calcWordRectList(CEDLine& line) {
    word_rects_.clear();
    Rect word_rect;
    bool word_flag = false;

    for (size_t i = 0; i < line.elementCount(); i++) {
        CEDChar * chr = dynamic_cast<CEDChar*> (line.elementAt(i));
        if (chr && !isSpace(*chr)) {
            if (word_flag) {
                word_rect |= chr->boundingRect();
            } else {
                word_flag = true;
                word_rect = chr->boundingRect();
            }
        } else {
            word_flag = false;
            word_rects_.push_back(word_rect);
            word_rect = Rect();
        }
    }

    if (word_flag)
        word_rects_.push_back(word_rect);
}

void DjvuTxtExporter::closeWord() {
    word_flag_ = false;
    writeWordEnd();
}

void DjvuTxtExporter::startWord() {
    word_flag_ = true;
    writeWordBegin();
}

void DjvuTxtExporter::writeCharacter(CEDChar& chr) {
    if (isSpace(chr))
        return;

    word_buffer_ << chr.alternativeAt(0).getChar();
}

void DjvuTxtExporter::writeCharacterBegin(CEDChar& chr) {
    if (isSpace(chr)) {
        if (word_flag_)
            closeWord();
    } else if (!word_flag_) {
        startWord();
    }
}

void DjvuTxtExporter::writeIndent() {
    if (indent_level_ < 1)
        return;

    buffer() << std::string(indent_level_ * 3, ' ');
}

void DjvuTxtExporter::writeLineBegin(CEDLine& line) {
    calcWordRectList(line);

    indent_level_++;
    writeIndent();
    buffer() << "(line ";
    writeRect(line.boundingRect());
    buffer() << '\n';
}

void DjvuTxtExporter::writeLineBreak() {
}

void DjvuTxtExporter::writeLineEnd(CEDLine&) {
    if (word_flag_)
        closeWord();

    writeIndent();
    indent_level_--;
    buffer() << ")\n";
}

void DjvuTxtExporter::writePageBegin(CEDPage& page) {
    buffer() << std::dec;
    buffer() << "(page 0 0 " << page.imageSize().width() << ' ' << page.imageSize().height()
            << "\n";
}

void DjvuTxtExporter::writePageEnd(CEDPage&) {
    buffer() << ")\n";
    flushBuffer();
}

void DjvuTxtExporter::writeParagraphBegin(CEDParagraph& par) {
    par.updateBoundingRect();
    indent_level_++;
    writeIndent();
    buffer() << "(para ";
    writeRect(par.boundingRect());
    buffer() << '\n';
}

void DjvuTxtExporter::writeParagraphEnd(CEDParagraph&) {
    writeIndent();
    buffer() << ")\n";
    indent_level_--;
}

void DjvuTxtExporter::writeRect(const Rect& r) {
    assert(page());

    int yoff = page()->imageSize().height();
    buffer() << r.left() << ' ' << yoff - r.top() << ' ' << r.right() << ' ' << yoff - r.bottom();
}

void DjvuTxtExporter::writeWordBegin() {
    indent_level_++;
    writeIndent();
    buffer() << "(word ";
    writeRect(word_rects_.front());
    word_rects_.pop_front();
    buffer() << " \"";
    word_buffer_.str("");
}

void DjvuTxtExporter::writeWordBuffer() {
    std::string word;

    if (isCharsetConversion())
        word = converter_.convert(word_buffer_.str());
    else
        word = word_buffer_.str();

    char old_fill = buffer().fill();
    buffer().fill('0');

    for (size_t i = 0, end = word.size(); i < end; i++) {
        int c = static_cast<unsigned char> (word[i]);

        if ('0' <= c && c <= 'z') {
            if (c == '"')
                buffer() << '\\';

            buffer() << (char) c;
        } else {
            buffer() << '\\' << std::setw(3) << std::oct << c;
        }
    }
    buffer().fill(old_fill);
    buffer() << std::dec;

}

void DjvuTxtExporter::writeWordEnd() {
    writeWordBuffer();
    buffer() << "\")\n";
    indent_level_--;
}

}
