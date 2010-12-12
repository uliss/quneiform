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

#include <iostream>
#include "rectexporter.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedpage.h"
#include "ced/cedpicture.h"

namespace cf {

static const int BAD_RECT_VALUE = 65535;

inline bool goodCharRect(const Rect& rc) {
    return rc.left() != -1 && //
            rc.left() != BAD_RECT_VALUE && //
            rc.right() != BAD_RECT_VALUE && //
            rc.top() != BAD_RECT_VALUE && //
            rc.bottom() != BAD_RECT_VALUE;
}

inline QRect cf2qt(const cf::Rect& rect) {
    return QRect(rect.left() - 1, rect.top() - 1, rect.width(), rect.height());
}

RectExporter::RectExporter(CEDPage * page)
    : GenericExporter(page, FormatOptions())
{
    setSkipPictures(false);
    setSkipEmptyLines(false);
    setSkipEmptyParagraphs(false);
}

void RectExporter::addCharBBox(CEDChar& chr) {
    Rect r = chr.boundingRect();

    // spaces have invalid bounding rectangle
    if (goodCharRect(r)) {
        chars_.append(cf2qt(r));

//        if (is_in_line_) {
//            line_bbox_ |= r;
//        }
//        // begin to determine line bounds
//        else {
//            line_bbox_ = r;
//            is_in_line_ = true;
//        }
    }
}

const RectExporter::RectList& RectExporter::chars() const {
    return chars_;
}

void RectExporter::collect() {
    doExport(std::cerr);
}

void RectExporter::doExport(std::ostream&) {
    Q_CHECK_PTR(page());

    page()->exportElement(*this);
}

const RectExporter::RectList& RectExporter::paragraphs() const {
    return paragraphs_;
}

const RectExporter::RectList& RectExporter::pictures() const {
    return pictures_;
}

void RectExporter::writeCharacterEnd(CEDChar& chr) {
    addCharBBox(chr);
}

void RectExporter::writeLineEnd(CEDLine& line) {
    lines_ << cf2qt(line.boundingRect());
}

void RectExporter::writeParagraphEnd(CEDParagraph& par) {
    paragraphs_ << cf2qt(par.boundingRect());
}

void RectExporter::writePicture(CEDPicture& pict) {
    pictures_ << cf2qt(pict.boundingRect());
}

}
