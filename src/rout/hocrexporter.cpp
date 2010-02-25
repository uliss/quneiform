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

#include "hocrexporter.h"
#include "ced/cedint.h"
#include "common/helper.h"

namespace CIF
{

const int BOXES_TO_RESERVE = 200;
const int BAD_RECT_VALUE = 65535;

inline bool goodCharRect(const Rect& rc) {
    return rc.left() != -1 and rc.left() != BAD_RECT_VALUE and rc.right() != BAD_RECT_VALUE
            and rc.top() != BAD_RECT_VALUE and rc.bottom() != BAD_RECT_VALUE;
}

HocrExporter::HocrExporter(CEDPage * page, const FormatOptions& opts) :
    HtmlExporter(page, opts), is_in_line_(false) {
    rects_.reserve(BOXES_TO_RESERVE);
}

void HocrExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr);
    Rect r;
    r.rleft() = chr->layout.left;
    r.rtop() = chr->layout.top;
    r.rright() = chr->layout.right;
    r.rbottom() = chr->layout.bottom;

    // spaces have not valid bounding rectangle
    if (goodCharRect(r)) {
        rects_.push_back(r);

        if (is_in_line_) {
            line_rect_ |= r;
        }
        // begin to determine line bounds
        else {
            line_rect_ = r;
            is_in_line_ = true;
        }
    }

    HtmlExporter::writeCharacter(os, chr);
}

void HocrExporter::writeLineBegin(std::ostream& os, CEDLine * line) {
    assert(line);

    writeFontStyle(os, 0);

    os << "  <span class=\"ocr_line\" id=\"line_" << numLines() << "\" " << "title=\"bbox "
            << line_rect_.left() << " " << line_rect_.top() << " " << line_rect_.right() << " "
            << line_rect_.bottom() << "\">";
}

void HocrExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    writeFontStyle(os, 0);
    os << "</span>";
    HtmlExporter::writeLineEnd(os, line);
    is_in_line_ = false;
    rects_.clear();
}

void HocrExporter::writeMeta(std::ostream& os) {
    HtmlExporter::writeMeta(os);

    Attributes attrs;
    attrs["name"] = "ocr-system";
    attrs["content"] = "cuneiform";
    writeSingleTag(os, "meta", attrs);
    os << "\n";
}

void HocrExporter::writePageBegin(std::ostream& os) {
    assert(page() and page()->imageName);
    HtmlExporter::writePageBegin(os);
    static int num_pages = 1;
    // example: <div class="ocr_page" title="image 'page-000.pbm'; bbox 0 0 4306 6064">
    os << "<div class=\"ocr_page\" id=\"page_" << num_pages << "\" " << "title=\"image '"
            << escapeHtmlSpecialChars(page()->imageName) << "'; bbox 0 0 "
            << page()->sizeOfImage.cx << " " << page()->sizeOfImage.cy << "\">\n";
    num_pages++;
}

void HocrExporter::writePageEnd(std::ostream& os) {
    os << "</div>\n";
    HtmlExporter::writePageEnd(os);
}

void HocrExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    HtmlExporter::writeParagraphBegin(os, par);
    os << "\n";
}

void HocrExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * par) {
    os << "\n";
    HtmlExporter::writeParagraphEnd(os, par);
}

}
