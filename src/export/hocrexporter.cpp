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
#include "ced/cedpicture.h"
#include "ced/cedchar.h"
#include "ced/cedpage.h"
#include "common/helper.h"
#include "common/filesystem.h"

namespace cf
{

static const int BOXES_TO_RESERVE = 200;
static const int BAD_RECT_VALUE = 65535;

static inline bool goodCharRect(const Rect& rc) {
    return rc.left() != -1 && rc.left() != BAD_RECT_VALUE && rc.right() != BAD_RECT_VALUE
            && rc.top() != BAD_RECT_VALUE && rc.bottom() != BAD_RECT_VALUE;
}

static inline std::string rectBBox(const Rect& rc) {
    std::ostringstream buf;
    buf << "bbox " << rc.left() << " " << rc.top() << " " << rc.right() << " " << rc.bottom();
    return buf.str();
}

static inline std::string rectBBoxes(const HocrExporter::RectList& lst) {
    std::ostringstream buf;
    buf << "x_bboxes";
    for (HocrExporter::RectList::const_iterator it = lst.begin(), end = lst.end(); it != end; ++it)
        buf << " " << it->left() << " " << it->top() << " " << it->right() << " " << it->bottom();
    return buf.str();
}

static inline std::string pageBBox(CEDPage& p, bool basename = false) {
    std::ostringstream buf;
    std::string path = p.imageName();

    if(basename)
        path = fs::baseName(path);

    buf << "image '" << escapeHtmlSpecialChars(path) << "'; bbox 0 0 "
            << p.imageSize().width() << " " << p.imageSize().height();
    return buf.str();
}

HocrExporter::HocrExporter(CEDPagePtr page, const FormatOptions& opts) :
    HtmlExporter(page, opts),
    is_in_line_(false)
{
    formatOptions().setShowAlternatives(false);
    formatOptions().useBold(false);
    formatOptions().useItalic(false);
    formatOptions().useFontSize(false);
    formatOptions().useUnderlined(false);
    formatOptions().useStyles(false);
    char_bboxes_.reserve(BOXES_TO_RESERVE);
}

void HocrExporter::addCharBBox(CEDChar& chr) {
    Rect r = chr.boundingRect();

    // spaces have invalid bounding rectangle
    if (goodCharRect(r)) {
        char_bboxes_.push_back(r);

        if (is_in_line_) {
            line_bbox_ |= r;
        }
        // begin to determine line bounds
        else {
            line_bbox_ = r;
            is_in_line_ = true;
        }
    }
}

void HocrExporter::clearCharBBoxes() {
    char_bboxes_.clear();
}

void HocrExporter::clearLineBBox() {
    line_bbox_ = Rect();
}

void HocrExporter::writeCharacter(CEDChar& chr) {
    addCharBBox(chr);
    HtmlExporter::writeCharacter(chr);
}

void HocrExporter::writeCharBBoxesInfo() {
    Attributes attrs;
    attrs["class"] = "ocr_info";
    attrs["title"] = rectBBoxes(char_bboxes_);

    writeSingleTag("span", attrs, "\n");
}

void HocrExporter::writeLineEnd(CEDLine& line) {
    Attributes attrs;
    attrs["class"] = "ocr_line";
    attrs["id"] = "line_" + toString(numLines());
    attrs["title"] = rectBBox(line_bbox_);
    writeStartTag("span", attrs);

    writeCharBBoxesInfo();
    HtmlExporter::writeLineEnd(line);

    writeCloseTag("span", "\n");
    is_in_line_ = false;
    clearCharBBoxes();
}

void HocrExporter::writeMeta() {
    HtmlExporter::writeMeta();

    Attributes attrs;
    attrs["name"] = "ocr-system";
    attrs["content"] = "cuneiform";
    writeSingleTag("meta", attrs, "\n");
}

void HocrExporter::writePageBegin(CEDPage& page) {
    HtmlExporter::writePageBegin(page);
    static int num_pages = 1;
    // example: <div class="ocr_page" title="image 'page-000.pbm'; bbox 0 0 4306 6064">

    Attributes attrs;
    attrs["class"] = "ocr_page";
    attrs["id"] = "page_" + toString(num_pages);
    attrs["title"] = pageBBox(page, formatOptions().isTestOutput());

    writeStartTag("div", attrs, "\n");

    num_pages++;
}

void HocrExporter::writePageEnd(CEDPage& page) {
    writeCloseTag("div", "\n");
    HtmlExporter::writePageEnd(page);
    flushBuffer();
}

void HocrExporter::writeParagraphBegin(CEDParagraph& par) {
    HtmlExporter::writeParagraphBegin(par);
    buffer() << '\n';
    clearCharBBoxes();
    clearLineBBox();

}

void HocrExporter::writePicture(CEDPicture& picture) {
    line_bbox_ = picture.boundingRect();
    HtmlExporter::writePicture(picture);
}

}
