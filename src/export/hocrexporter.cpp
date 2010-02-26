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
    return rc.left() != -1 && rc.left() != BAD_RECT_VALUE && rc.right() != BAD_RECT_VALUE
            && rc.top() != BAD_RECT_VALUE && rc.bottom() != BAD_RECT_VALUE;
}

HocrExporter::HocrExporter(CEDPage * page, const FormatOptions& opts) :
    HtmlExporter(page, opts), is_in_line_(false) {
    rects_.reserve(BOXES_TO_RESERVE);
    formatOptions().setPreserveLineHyphens(true);
}

void HocrExporter::addCharBBox(CEDChar * chr) {
    assert(chr);
    Rect r = chr->boundingRect();

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
}

void HocrExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    addCharBBox(chr);
    HtmlExporter::writeCharacter(os, chr);
}

void HocrExporter::writeCharBBoxesInfo(std::ostream& os) {
    os << "\n  <span class=\"ocr_info\" title=\"x_bboxes";
    for (RectList::iterator it = rects_.begin(), end = rects_.end(); it != end; ++it)
        os << " " << it->left() << " " << it->top() << " " << it->right() << " " << it->bottom();
    os << "\"></span>\n";
}

void HocrExporter::writeLineBegin(std::ostream& /*os*/, CEDLine * /*line*/) {
    writeFontStyle(lineBuffer(), 0);
}

void HocrExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    os << "  <span class=\"ocr_line\" id=\"line_" << numLines() << "\" " << "title=\"bbox "
            << line_rect_.left() << " " << line_rect_.top() << " " << line_rect_.right() << " "
            << line_rect_.bottom() << "\">";

    writeCharBBoxesInfo(os);
    writeFontStyle(lineBuffer(), 0);
    HtmlExporter::writeLineEnd(os, line);
    os << "</span>\n";
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
    assert(page() && page()->imageName);
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
    rects_.clear();
    line_rect_ = Rect();
}

}
