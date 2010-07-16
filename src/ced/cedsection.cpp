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

namespace CIF
{

CEDSection::CEDSection() {
    sectionBreak = 0;
    width = 0;
    height = 0;
    orientation = 0;
    headerY = 0;
    footerY = 0;
    numSnakeCols = 0;
    lineBetCol = 0;
    colInfo = 0;
}

CEDSection::~CEDSection() {
    if (colInfo)
        delete[] colInfo;
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

CEDFrame * CEDSection::createFrame(CEDColumn * col, const Rect& rect, char position,
        int borderSpace, int dxfrtextx, int dxfrtexty) {
    CEDFrame * frame = new CEDFrame;
    col->addElement(frame);
    frame->setBoundingRect(rect);
    frame->setPosition(position);
    frame->setBorderSpace(borderSpace);
    frame->dxfrtextx = dxfrtextx;
    frame->dxfrtexty = dxfrtexty;
    return frame;
}

CEDParagraph * CEDSection::CreateParagraph(BlockElement * cont, align_t align, const Rect& indent,
        int UserNum, int FlagBorder, EDSIZE interval, const Rect& layout, const Color& color,
        const Color& shading, int spaceBetweenLines) {
    CEDParagraph * para = new CEDParagraph;
    cont->addElement(para);
    para->setAlign(align);
    para->setPaddingLeft(indent.x());
    para->setPaddingRight(indent.pt1().x());
    para->setIndent(indent.y());
    para->setColor(color);
    para->setBackgroundColor(shading);
    para->setLineSpace(spaceBetweenLines);
    para->setBoundingRect(layout);
    para->setUserNumber(UserNum);
    para->setMarginBottom(interval.cy);
    para->setMarginTop(interval.cx);
    para->border = FlagBorder;
    return para;
}

}
