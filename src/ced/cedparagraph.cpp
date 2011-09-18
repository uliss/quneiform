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
#include "cedexporter.h"

namespace cf
{

CEDParagraph::CEDParagraph() :
    BlockElement(), align_(ALIGN_LEFT), line_space_(-1), indent_(0), padding_left_(0),
            padding_right_(0), user_number_(0) {
}

align_t CEDParagraph::align() const {
    return align_;
}

void CEDParagraph::exportElement(CEDExporter& exp) {
    exp.exportParagraph(*this);
}

int CEDParagraph::indent() const {
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

void CEDParagraph::setIndent(int value) {
    indent_ = value;
}

void CEDParagraph::setLineSpace(int value) {
    line_space_ = value;
}

void CEDParagraph::setPaddingLeft(int value) {
    padding_left_ = value;
}

void CEDParagraph::setPaddingRight(int value) {
    padding_right_ = value;
}

void CEDParagraph::setUserNumber(int value) {
    user_number_ = value;
}

}
