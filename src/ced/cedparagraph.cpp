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
#include "cedparagraph.h"
#include "cedline.h"
#include "ced_struct.h"

namespace CIF
{

CEDParagraph::CEDParagraph() :
    BlockElement(), align_(ALIGN_LEFT), line_space_(-1) {
    type = 0;
    userNumber = 0;
    border = 0;
    interval.cx = interval.cy = 0;
    descriptor = 0;
    leftBrdrWidth = 0;
    rightBrdrWidth = 0;
    topBrdrWidth = 0;
    bottomBrdrWidth = 0;
    brdrBtw = 0;
    internal_number_ = 0;
}

CEDParagraph::~CEDParagraph() {
}

align_t CEDParagraph::align() const {
    return align_;
}

const Rect& CEDParagraph::indent() const {
    return indent_;
}

CEDLine * CEDParagraph::lineAt(size_t pos) {
    return dynamic_cast<CEDLine*> (elementAt(pos));
}

size_t CEDParagraph::lineCount() const {
    return elementCount();
}

int CEDParagraph::lineSpace() const {
    return line_space_;
}

void CEDParagraph::addLine(CEDLine * line) {
    addElement(line);
}

void CEDParagraph::setAlign(align_t align) {
    align_ = align;
}

void CEDParagraph::setIndent(const Rect& ind) {
    indent_ = ind;
}

void CEDParagraph::setLineSpace(int value) {
    line_space_ = value;
}

}
