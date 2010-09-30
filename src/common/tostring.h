/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef TOSTRING_H_
#define TOSTRING_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include "point.h"
#include "rect.h"
#include "size.h"
#include "color.h"

namespace cf
{

template<class T>
std::ostream& operator<<(std::ostream& os, const PointImpl<T>& point) {
    os << "Point(" << point.x() << "," << point.y() << ")";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const RectImpl<T>& rect) {
    os << "Rect(" << rect.pt0() << ", " << rect.pt1() << ")" << " width:" << rect.width()
            << "; height:" << rect.height();
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const SizeImpl<T>& size) {
    os << "Size(" << size.width() << "," << size.height();
    return os;
}

template<class T>
std::string toBBox(const RectImpl<T>& rect) {
    std::ostringstream os;
    os << rect.left() << " " << rect.top() << " " << rect.right() << " " << rect.bottom();
    return os.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const ColorImpl<T>& color) {
    char old_fill = os.fill();
    os.fill('0');
    os << "Color(0x" << std::hex << std::setw(2) << (int) color.red() << std::setw(2) << (int) color.green()
            << std::setw(2) << (int) color.blue() << ")" << std::dec;
    os.fill(old_fill);
    return os;
}

}

#endif /* TOSTRING_H_ */
