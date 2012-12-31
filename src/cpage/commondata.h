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

#ifndef COMMONDATA_H
#define COMMONDATA_H

#include "globus.h"
#include "cpagedefs.h"
#include "common/point.h"
#include "common/rect.h"

namespace cf {

typedef PointImpl<int32_t> Point32;

namespace cpage {

class CommonData
{
    CDataType type_; //Текст, Картинка, Таблица;
public:
    CommonData();
    int16_t count;
    Point32 Vertex[CPAGE_MAXCORNER];

    void addVertex(const Point& pt);

    uint32_t flags() const {
        return flags_;
    }

    int number() const;

    Rect rect() const {
        return Rect(Vertex[0], Vertex[2]);
    }

    void setFlag(uint32_t flag)  {
        flags_ |= flag;
    }

    void setFlags(uint32_t flags) {
        flags_ = flags;
    }

    void setNumber(int n);

    void setRect(const Rect& r) {
        Vertex[0] = r.leftTop();
        Vertex[1] = r.rightTop();
        Vertex[2] = r.rightBottom();
        Vertex[3] = r.leftBottom();
        count = 4;
    }

    void setType(CDataType type);
    CDataType type() const;

    Point vertexAt(size_t pos) const;
    size_t vertexCount() const;
private:
    int16_t number_; // порядковый номер
    uint32_t flags_;
};

}
}

#endif // COMMONDATA_H
