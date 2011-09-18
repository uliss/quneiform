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

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cedframe.h"
#include "cedsection.h"
#include "cedcolumn.h"
#include "cedparagraph.h"
#include "cedexporter.h"

namespace cf
{

CEDSection::CEDSection() :
    header_y_(0), footer_y_(0), line_between_columns_(false), section_break_(false),
            orientation_(0) {
}

void CEDSection::addColInfo(int width, int space) {
    col_wd_.push_back(width);
    col_space_.push_back(space);
}

void CEDSection::addColumn(CEDColumn * col) {
    addElement(col);
}

CEDColumn * CEDSection::columnAt(size_t pos) {
    return static_cast<CEDColumn*> (elementAt(pos));
}

size_t CEDSection::columnCount() const {
    return elementCount();
}

void CEDSection::exportElement(CEDExporter& exp) {
    exp.exportSection(*this);
}

bool CEDSection::lineBetweenColumns() const {
    return line_between_columns_;
}

bool CEDSection::sectionBreak() const {
    return section_break_;
}

void CEDSection::setFooterY(int value) {
    footer_y_ = value;
}

void CEDSection::setHeaderY(int value) {
    header_y_ = value;
}

void CEDSection::setLineBetweenColumns(bool value) {
    line_between_columns_ = value;
}

void CEDSection::setOrientation(char value) {
    assert(value == 1 || value == 0 );
    orientation_ = value;
}

void CEDSection::setSectionBreak(bool value) {
    section_break_ = value;
}

CEDParagraph * CEDSection::createParagraph(BlockElement * cont, align_t align, const Rect& indent,
        int UserNum, int FlagBorder, const Rect& layout, const Color& color,
        const Color& shading) {
    CEDParagraph * para = new CEDParagraph;
    cont->addElement(para);
    para->setAlign(align);
    para->setPaddingLeft(indent.x());
    para->setPaddingRight(indent.pt1().x());
    para->setIndent(indent.y());
    para->setColor(color);
    para->setBackgroundColor(shading);
    para->setBoundingRect(layout);
    para->setUserNumber(UserNum);
    return para;
}

}
