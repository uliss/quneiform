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

#include "cedcolumn.h"
#include "cedsection.h"
#include "cedexporter.h"

namespace cf
{

CEDColumn::CEDColumn(CEDSection * parent) :
    BlockElement(parent),
    width_(0),
    space_(0)
{}

CEDColumn * CEDColumn::clone() const
{
    return new CEDColumn(*this);
}

CEDColumn::CEDColumn(const CEDColumn& col) :
    BlockElement(col),
    width_(col.width_),
    space_(col.space_)
{}


void CEDColumn::exportElement(CEDExporter& exp) {
    exp.exportColumn(*this);
}

void CEDColumn::setSpace(int space) {
    space_ = space;
}

void CEDColumn::setWidth(int width) {
    width_ = width;
}

int CEDColumn::space() const {
    return space_;
}

int CEDColumn::width() const {
    return width_;
}

}
