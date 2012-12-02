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
#include "cfutils.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedpage.h"
#include "ced/cedcolumn.h"
#include "ced/cedpicture.h"

namespace cf {

static const int BAD_RECT_VALUE = 65535;

// spaces have invalid bounding rectangle
inline bool isValid(const cf::Rect& rc) {
    return rc.left() != -1 && //
            rc.left() != BAD_RECT_VALUE && //
            rc.right() != BAD_RECT_VALUE && //
            rc.top() != BAD_RECT_VALUE && //
            rc.bottom() != BAD_RECT_VALUE;
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

const RectExporter::BlockList& RectExporter::chars() const {
    return chars_;
}

void RectExporter::collect() {
    doExport(std::cerr);
}

const RectExporter::BlockList& RectExporter::columns() const {
    return columns_;
}

void RectExporter::doExport(std::ostream&) {
    Q_CHECK_PTR(page());

    page()->exportElement(*this);
}

const RectExporter::BlockList& RectExporter::lines() const {
    return lines_;
}

const RectExporter::BlockList& RectExporter::paragraphs() const {
    return paragraphs_;
}

const RectExporter::BlockList& RectExporter::pictures() const {
    return pictures_;
}

const RectExporter::BlockList& RectExporter::sections() const {
    return sections_;
}

void RectExporter::writeCharacterEnd(CEDChar& chr)
{
    // skip spaces
    if(!isValid(chr.boundingRect()))
        return;

    QRect current_char = toQRect(chr.boundingRect());
    chars_.append(Block(BLOCK_CHAR, current_char));

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

void RectExporter::writeColumnEnd(CEDColumn&)
{
    columns_.append(Block(BLOCK_COLUMN, current_column_));

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

void RectExporter::writeLineEnd(CEDLine&)
{
    // start new paragraph
    if(par_begin_) {
        current_par_ = current_line_;
        par_begin_ = false;
    }
    else
        current_par_ |= current_line_;

    lines_.append(Block(BLOCK_LINE, current_line_));
}

void RectExporter::writeParagraphBegin(CEDParagraph&) {
    par_begin_ = true;
}

void RectExporter::writeParagraphEnd(CEDParagraph&)
{
    paragraphs_.append(Block(BLOCK_PARAGRAPH, current_par_));

    // start new column
    if(column_begin_) {
        current_column_ = current_par_;
        column_begin_ = false;
    }
    else
        current_column_ |= current_par_;
}

void RectExporter::writePicture(CEDPicture& pict)
{
    pictures_.append(Block(BLOCK_PICTURE, toQRect(pict.boundingRect())));
}

void RectExporter::writeSectionBegin(CEDSection&) {
    section_begin_ = true;
}

void RectExporter::writeSectionEnd(CEDSection&)
{
    sections_.append(Block(BLOCK_SECTION, current_section_));
}

}
