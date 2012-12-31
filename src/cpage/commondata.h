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
namespace cpage {

class CommonData
{
    CDataType type_; //Текст, Картинка, Таблица;
public:
    CommonData();

    int16_t number; // порядковый номер
    int16_t count;
    cf::Point Vertex[CPAGE_MAXCORNER];

    uint32_t flags() const {
        return Flags;
    }

    void setFlag(uint32_t flag)  {
        Flags |= flag;
    }

    void setFlags(uint32_t flags) {
        Flags = flags;
    }

    void addVertex(const Point& pt)
    {
        Vertex[count] = pt;
        count++;
    }

    Rect rect() const {
        return Rect(Vertex[0], Vertex[2]);
    }

    void setRect(const Rect& r)
    {
        Vertex[0] = r.leftTop();
        Vertex[1] = r.rightTop();
        Vertex[2] = r.rightBottom();
        Vertex[3] = r.leftBottom();
        count = 4;
    }

    void setType(CDataType type);
private:
    uint32_t Flags;
};

}
}

#endif // COMMONDATA_H
