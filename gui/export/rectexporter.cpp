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
#include "ced/cedcolumn.h"
#include "ced/cedpicture.h"

namespace cf {

static const int BAD_RECT_VALUE = 65535;

// spaces have invalid bounding rectangle
inline bool isValid(const QRect& rc) {
    return rc.left() != -1 && //
            rc.left() != BAD_RECT_VALUE && //
            rc.right() != BAD_RECT_VALUE && //
            rc.top() != BAD_RECT_VALUE && //
            rc.bottom() != BAD_RECT_VALUE;
}

inline QRect cf2qt(const cf::Rect& rect) {
    return QRect(rect.left(), rect.top() - 1, rect.width(), rect.height());
}

RectExporter::RectExporter(CEDPagePtr page)
    : GenericExporter(page, FormatOptions()),
    column_begin_(true),
    line_begin_(true),
    par_begin_(true),
    section_begin_(true)
{
    setSkipPictures(false);
    setSkipEmptyLines(false);
    setSkipEmptyParagraphs(false);
}

const RectExporter::RectList& RectExporter::chars() const {
    return chars_;
}

void RectExporter::collect() {
    doExport(std::cerr);
}

const RectExporter::RectList& RectExporter::columns() const {
    return columns_;
}

void RectExporter::doExport(std::ostream&) {
    Q_CHECK_PTR(page());

    page()->exportElement(*this);
}

const RectExporter::RectList& RectExporter::lines() const {
    return lines_;
}

const RectExporter::RectList& RectExporter::paragraphs() const {
    return paragraphs_;
}

const RectExporter::RectList& RectExporter::pictures() const {
    return pictures_;
}

const RectExporter::RectList& RectExporter::sections() const {
    return sections_;
}

void RectExporter::writeCharacterEnd(CEDChar& chr) {
    QRect current_char = cf2qt(chr.boundingRect());

    if(isValid(current_char)) {
        chars_.append(current_char);
    }
    // skip spaces
    else
        return;

    if(line_begin_) {
        current_line_ = current_char;
        line_begin_ = false;
    }
    else
        current_line_ |= current_char;
}

void RectExporter::writeColumnBegin(CEDColumn&) {
    column_begin_ = true;
}

void RectExporter::writeColumnEnd(CEDColumn&) {
    columns_.append(current_column_);

    if(section_begin_) {
        current_section_ = current_column_;
        section_begin_ = false;
    }
    else
        current_section_ |= current_column_;
}

void RectExporter::writeLineBegin(CEDLine&) {
    line_begin_ = true;
}

void RectExporter::writeLineEnd(CEDLine&) {
    // start new paragraph
    if(par_begin_) {
        current_par_ = current_line_;
        par_begin_ = false;
    }
    else
        current_par_ |= current_line_;

    lines_.append(current_line_);
}

void RectExporter::writeParagraphBegin(CEDParagraph&) {
    par_begin_ = true;
}

void RectExporter::writeParagraphEnd(CEDParagraph&) {
    paragraphs_.append(current_par_);

    // start new column
    if(column_begin_) {
        current_column_ = current_par_;
        column_begin_ = false;
    }
    else
        current_column_ |= current_par_;
}

void RectExporter::writePicture(CEDPicture& pict) {
    QRect r = cf2qt(pict.boundingRect());
    pictures_.append(r);
}

void RectExporter::writeSectionBegin(CEDSection&) {
    section_begin_ = true;
}

void RectExporter::writeSectionEnd(CEDSection&) {
    sections_.append(current_section_);
}

}
