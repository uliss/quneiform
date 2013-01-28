/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include "tablecell.h"

namespace cf {
namespace cpage {

TableCell::TableCell() :
    number_(0),
    geom_count_(0),
    fragm_number_(0)
{
}

int TableCell::number() const
{
    return number_;
}

void TableCell::setNumber(int n)
{
    number_ = n;
}

Point TableCell::point() const
{
    return coord_;
}

void TableCell::setPoint(const Point& pt)
{
    coord_ = pt;
}

} // namespace cpage
} // namespace cf
