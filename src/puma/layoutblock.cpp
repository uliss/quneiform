/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include "layoutblock.h"
#include "common/tostring.h"

namespace cf
{

LayoutBlock::LayoutBlock(const Rect& r, Type t) :
    rect_(r),
    type_(t)
{
}

bool LayoutBlock::isImage() const
{
    return type_ == IMAGE;
}

bool LayoutBlock::isText() const
{
    return type_ == TEXT;
}


Rect LayoutBlock::rect() const
{
    return rect_;
}

void LayoutBlock::setRect(const Rect& r)
{
    rect_ = r;
}

LayoutBlock::Type LayoutBlock::type() const
{
    return type_;
}

void LayoutBlock::setType(LayoutBlock::Type t)
{
    type_ = t;
}

std::ostream& operator<<(std::ostream& os, const LayoutBlock& block)
{
    os << "Layout block: ";

    switch(block.type()) {
    case LayoutBlock::IMAGE:
        os << "image";
        break;
    case LayoutBlock::TEXT:
        os << "text";
        break;
    case LayoutBlock::TABLE:
        os << "table";
        break;
    default:
        os << "invalid";
    }

    os << "\n";
    Rect r = block.rect();
    os << "    " << r.x() << ',' << r.y() << ' ' << r.width() << 'x' << r.height() << "\n";
    return os;
}


}
