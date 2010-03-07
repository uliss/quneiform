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

inline std::string rectBBox(const Rect& rc) {
    std::ostringstream buf;
    buf << "bbox " << rc.left() << " " << rc.top() << " " << rc.right() << " " << rc.bottom();
    return buf.str();
}

inline std::string rectBBoxes(const HocrExporter::RectList& lst) {
    std::ostringstream buf;
    buf << "x_bboxes";
    for (HocrExporter::RectList::const_iterator it = lst.begin(), end = lst.end(); it != end; ++it)
        buf << " " << it->left() << " " << it->top() << " " << it->right() << " " << it->bottom();
    return buf.str();
}

inline std::string pageBBox(CEDPage * p) {
    assert(p);
    std::ostringstream buf;
    buf << "image '" << escapeHtmlSpecialChars(p->imageName) << "'; bbox 0 0 " << p->sizeOfImage.cx
            << " " << p->sizeOfImage.cy;
    return buf.str();
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
    Attributes attrs;
    attrs["class"] = "ocr_info";
    attrs["title"] = rectBBoxes(rects_);
    writeSingleTag(os, "span", attrs, "\n");
}

void HocrExporter::writeLineBegin(std::ostream& /*os*/, CEDLine * /*line*/) {
    writeFontStyle(lineBuffer(), 0);
}

void HocrExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    Attributes attrs;
    attrs["class"] = "ocr_line";
    attrs["id"] = "line_" + toString(numLines());
    attrs["title"] = rectBBox(line_rect_);
    writeStartTag(os, "span", attrs, "\n");

    writeCharBBoxesInfo(os);
    writeFontStyle(lineBuffer(), 0);
    HtmlExporter::writeLineEnd(os, line);
    writeCloseTag(os, "span", "\n");
    is_in_line_ = false;
    rects_.clear();

}

void HocrExporter::writeMeta(std::ostream& os) {
    HtmlExporter::writeMeta(os);

    Attributes attrs;
    attrs["name"] = "ocr-system";
    attrs["content"] = "cuneiform";
    writeSingleTag(os, "meta", attrs, "\n");
}

void HocrExporter::writePageBegin(std::ostream& os, CEDPage * page) {
    assert(page && page->imageName);
    HtmlExporter::writePageBegin(os, page);
    static int num_pages = 1;
    // example: <div class="ocr_page" title="image 'page-000.pbm'; bbox 0 0 4306 6064">
    Attributes attrs;
    attrs["class"] = "ocr_page";
    attrs["id"] = "page_" + toString(num_pages);
    attrs["title"] = pageBBox(page);
    writeStartTag(os, "div", attrs, "\n");
    num_pages++;
}

void HocrExporter::writePageEnd(std::ostream& os, CEDPage * page) {
    writeCloseTag(os, "div", "\n");
    HtmlExporter::writePageEnd(os, page);
}

void HocrExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    HtmlExporter::writeParagraphBegin(os, par);
    os << "\n";
    rects_.clear();
    line_rect_ = Rect();
}

}
