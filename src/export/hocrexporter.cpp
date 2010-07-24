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
#include "xmltag.h"
#include "ced/cedpicture.h"
#include "ced/cedchar.h"
#include "ced/cedpage.h"
#include "common/helper.h"

namespace CIF
{

static const int BOXES_TO_RESERVE = 200;
static const int BAD_RECT_VALUE = 65535;

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

inline std::string pageBBox(CEDPage& p) {
    std::ostringstream buf;
    buf << "image '" << escapeHtmlSpecialChars(p.imageName()) << "'; bbox 0 0 "
            << p.imageSize().width() << " " << p.imageSize().height();
    return buf.str();
}

HocrExporter::HocrExporter(CEDPage * page, const FormatOptions& opts) :
    HtmlExporter(page, opts), is_in_line_(false) {
    rects_.reserve(BOXES_TO_RESERVE);
    formatOptions().setPreserveLineHyphens(true);
}

void HocrExporter::addCharBBox(CEDChar& chr) {
    Rect r = chr.boundingRect();

    // spaces have invalid bounding rectangle
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

void HocrExporter::writeCharacter(CEDChar& chr) {
    addCharBBox(chr);
    HtmlExporter::writeCharacter(chr);
}

void HocrExporter::writeCharBBoxesInfo() {
    XmlTag span("span");
    span["class"] = "ocr_info";
    span["title"] = rectBBoxes(rects_);
    outputStream() << span << "\n";
}

void HocrExporter::writeLineEnd(CEDLine& line) {
    XmlTag span("span");
    span["class"] = "ocr_line";
    span["id"] = "line_" + toString(numLines());
    span["title"] = rectBBox(line_rect_);
    span.writeBeginNL(outputStream());

    writeCharBBoxesInfo();
    //    resetFontStyle(os);
    HtmlExporter::writeLineEnd(line);

    span.writeEndNL(outputStream());
    is_in_line_ = false;
    rects_.clear();
}

void HocrExporter::writeMeta() {
    HtmlExporter::writeMeta();

    XmlTag meta("meta");
    meta["name"] = "ocr-system";
    meta["content"] = "cuneiform";
    outputStream() << meta << "\n";
}

void HocrExporter::writePageBegin(CEDPage& page) {
    HtmlExporter::writePageBegin(page);
    static int num_pages = 1;
    // example: <div class="ocr_page" title="image 'page-000.pbm'; bbox 0 0 4306 6064">
    XmlTag div("div");
    div["class"] = "ocr_page";
    div["id"] = "page_" + toString(num_pages);
    div["title"] = pageBBox(page);
    div.writeBeginNL(outputStream());
    num_pages++;
}

void HocrExporter::writePageEnd(CEDPage& page) {
    writeCloseTag("div", "\n");
    HtmlExporter::writePageEnd(page);
}

void HocrExporter::writeParagraphBegin(CEDParagraph& par) {
    HtmlExporter::writeParagraphBegin(par);
    outputStream() << "\n";
    rects_.clear();
    line_rect_ = Rect();
}

void HocrExporter::writePicture(CEDPicture& picture) {
    line_rect_ = picture.boundingRect();
    HtmlExporter::writePicture(picture);
}

}
