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
public:
    CommonData();

    void addVertex(int x, int y) { addVertex(Point(x, y)); }
    void addVertex(const Point& pt);
    void clearVertexes();
    void copyVertexes(const Picture& pict);

    int calcHeight() const;
    void crossBy0(const CommonData& rect);
    void crossBy1(const CommonData& rect);
    void crossBy2(const CommonData& rect);
    void crossBy3(const CommonData& rect);

    uint32_t flags() const {
        return flags_;
    }

    void insertVertex(size_t pos, const Point& p);
    void insertBottom(const Rect& r);
    void insertLeft(const Rect& rect);
    void insertRight(const Rect& rect);
    void insertTop(const Rect& rect);
    int isInPoly(const Point& pt) const;
    int isInPoly(const Point16& pt) const;
    bool isRect() const;

    void moveVertexX(size_t pos, int dx);
    void moveVertexY(size_t pos, int dy);

    int number() const;

    Rect rect() const {
        return Rect(vertex_[0], vertex_[2]);
    }

    template<int T>
    void rotateVertexToIdeal(size_t pos, int skew);
    template<int T>
    void rotateVertexToReal(size_t pos, int skew);
    void rotateVertexesToIdeal(int skew);

    void setFlag(uint32_t flag)  {
        flags_ |= flag;
    }

    void setFlags(uint32_t flags) {
        flags_ = flags;
    }

    void setNumber(int n);
    void setRect(const Rect& r);
    void setType(CDataType type);
    void setVertex(size_t pos, const Point& pt);
    void setVertexX(size_t pos, int x);
    void setVertexY(size_t pos, int y);
    CDataType type() const;
    Point& vertexAt(size_t pos);
    const Point& vertexAt(size_t pos) const;
    int vertexX(size_t pos) const;
    int vertexY(size_t pos) const;
    size_t vertexCount() const;
private:
    CDataType type_; //Текст, Картинка, Таблица;
    uint16_t count_;
    int16_t number_; // порядковый номер
    uint32_t flags_;
    Point32 vertex_[CPAGE_MAXCORNER];
};

template<int T>
void CommonData::rotateVertexToIdeal(size_t pos, int skew)
{
    int x = vertex_[pos].x();
    int y = vertex_[pos].y();
    x = (x + y * skew / T);
    y = (y - x * skew / T);
    vertex_[pos].set(x, y);
}

template<int T>
void CommonData::rotateVertexToReal(size_t pos, int skew)
{
    int x = vertex_[pos].x();
    int y = vertex_[pos].y();
    y = (y + x * skew / T);
    x = (x - y * skew / T);
    vertex_[pos].set(x, y);
}


}
}

#endif // COMMONDATA_H
