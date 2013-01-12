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

#include "commondata.h"
#include "picture.h"
#include "cpage_debug.h"

namespace cf {

namespace cpage {

CommonData::CommonData() :
    type_(0),
    count_(0),
    number_(0),
    flags_(0)
{
}

void CommonData::addVertex(const Point &pt)
{
    vertex_[count_] = pt;
    count_++;
}

void CommonData::clearVertexes()
{
    count_ = 0;
}

void CommonData::copyVertexes(const Picture& pict)
{
    for(size_t i = 0; i < pict.cornerCount(); i++)
        addVertex(pict.cornerAt(i));
}

int CommonData::calcHeight() const
{
    if(!count_)
        return 0;

    int min_top = vertex_[0].y();
    int max_bottom = min_top;

    for(int i = 1; i < count_; i++) {
        Point p = vertex_[i];
        min_top = std::min(min_top, p.y());
        max_bottom = std::max(max_bottom, p.y());
    }

    return max_bottom - min_top;
}

void CommonData::crossBy0(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count_ - 2; i++) {
        if ((vertex_[i].y() < rect.vertex_[0].y())
                && (vertex_[i + 1].x() > rect.vertex_[0].x())
                && (vertex_[i + 1].y() > rect.vertex_[0].y())
                && (vertex_[i + 2].x() < rect.vertex_[0].x())) {
            point.ry() = rect.vertex_[0].y();
            point.rx() = vertex_[i].x();
            insertVertex(i + 1, point);
            vertex_[i + 2] = rect.vertex_[0];
            point.ry() = vertex_[i + 3].y();
            point.rx() = rect.vertex_[0].x();
            insertVertex(i + 3, point);
            break;
        }
    }
}

void CommonData::crossBy1(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count_ - 2; i++) { //STEPA_AM |
        if ((vertex_[i].x() > rect.vertex_[1].x())
                && (vertex_[i + 1].x() < rect.vertex_[1].x())
                && (vertex_[i + 1].y() > rect.vertex_[1].y())
                && (vertex_[i + 2].y() < rect.vertex_[1].y())) {
            point.rx() = rect.vertex_[1].x();
            point.ry() = vertex_[i].y();
            insertVertex(i + 1, point);
            vertex_[i + 2] = rect.vertex_[1];
            point.rx() = vertex_[i + 3].x();
            point.ry() = rect.vertex_[1].y();
            insertVertex(i + 3, point);
            return;
        }
    }

    if ((vertex_[count_ - 2].x() > rect.vertex_[1].x())
            && (vertex_[count_ - 1].x() < rect.vertex_[1].x())
            && (vertex_[count_ - 1].y() > rect.vertex_[1].y())
            && (vertex_[0].y() < rect.vertex_[1].y())) {
        point.rx() = rect.vertex_[1].x();
        point.ry() = vertex_[count_ - 2].y();
        insertVertex(count_ - 1, point);
        insertVertex(count_ - 1, rect.vertex_[1]);
        point.rx() = vertex_[0].x();
        point.ry() = rect.vertex_[1].y();
        vertex_[count_ - 1] = point;
    }
}

void CommonData::crossBy2(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count_ - 2; i++) {
        if ((vertex_[i].y() > rect.vertex_[2].y())
                && (vertex_[i + 1].x() < rect.vertex_[2].x())
                && (vertex_[i + 1].y() < rect.vertex_[2].y())
                && (vertex_[i + 2].x() > rect.vertex_[2].x())) {
            point.ry() = rect.vertex_[2].y();
            point.rx() = vertex_[i].x();
            insertVertex(i + 1, point);
            vertex_[i + 2] = rect.vertex_[2];
            point.ry() = vertex_[i + 3].y();
            point.rx() = rect.vertex_[2].x();
            insertVertex(i + 3, point);
            return;
        }
    }

    if ((vertex_[count_ - 1].y() > rect.vertex_[2].y())
            && (vertex_[0].x() < rect.vertex_[2].x())
            && (vertex_[0].y() < rect.vertex_[2].y())
            && (vertex_[1].x() > rect.vertex_[2].x())) {
        //Special case - use with cautions :)
        point.ry() = vertex_[0].y();
        point.rx() = rect.vertex_[2].x();
        insertVertex(1, point);
        point.ry() = rect.vertex_[2].y();
        point.rx() = vertex_[0].x();
        insertVertex(1, rect.vertex_[2]);
        vertex_[0] = point;
    }
}

void CommonData::crossBy3(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count_ - 2; i++) {
        if ((vertex_[i].x() < rect.vertex_[3].x())
                && (vertex_[i + 1].x() > rect.vertex_[3].x())
                && (vertex_[i + 1].y() < rect.vertex_[3].y())
                && (vertex_[i + 2].y() > rect.vertex_[3].y())) {
            point.rx() = rect.vertex_[3].x();
            point.ry() = vertex_[i].y();
            insertVertex(i + 1, point);
            vertex_[i + 2] = rect.vertex_[3];
            point.rx() = vertex_[i + 3].x();
            point.ry() = rect.vertex_[3].y();
            insertVertex(i + 3, point);
            break;
        }
    }
}

bool CommonData::hasVertex(const Point& pt) const
{
    return std::find(vertex_, vertex_ + count_, pt) != (vertex_ + count_);
}

void CommonData::insertVertex(size_t pos, const Point& p)
{
    count_++;

    for (size_t i = count_ - 1; i >= pos && i > 0; i--)
        vertex_[i] = vertex_[i - 1];

    vertex_[pos] = p;
}

static inline bool isBetweenX(const Rect& r, const Point& pt1, const Point& pt2)
{
    return pt1.x() < r.left() && pt2.x() > r.right();
}

static inline bool isBetweenY(const Rect& r, const Point& pt1, const Point& pt2)
{
    return pt1.y() < r.top() && pt2.y() > r.bottom();
}

static inline bool isRectLower(const Rect& r, const Point& pt)
{
    return pt.y() < r.bottom();
}

void CommonData::insertBottom(const Rect& r)
{
    for (int i = 0; i < count_ - 1; i++) {
        Point v = vertex_[i];

        if (isBetweenX(r, vertex_[i], vertex_[i + 1])
                && isRectLower(r, v)) {
            insertVertex(i + 1, Point(r.left(), v.y()));
            insertVertex(i + 2, r.leftBottom());
            insertVertex(i + 3, r.rightBottom());
            insertVertex(i + 4, Point(r.right(), v.y()));
            break;
        }
    }
}

void CommonData::insertLeft(const Rect& rect)
{
    for (int i = 0; i < count_ - 1; i++) {
        Point v = vertex_[i];

        if ((v.x() > rect.left())
                && isBetweenY(rect, vertex_[i], vertex_[i + 1])) {
            insertVertex(i + 1, Point(v.x(), rect.top()));
            insertVertex(i + 2, rect.leftTop());
            insertVertex(i + 3, rect.leftBottom());
            insertVertex(i + 4, Point(v.x(), rect.bottom()));
            break;
        }
    }
}

void CommonData::insertRight(const Rect& rect)
{
    int i = 0;

    for (i = 0; i < count_ - 1; i++) {
        Point v = vertex_[i];
        if ((v.x() < rect.right())
                && isBetweenY(rect, vertex_[i + 1], vertex_[i])) {
            insertVertex(i + 1, Point(v.x(), rect.bottom()));
            insertVertex(i + 2, rect.rightBottom());
            insertVertex(i + 3, rect.rightTop());
            insertVertex(i + 4, Point(v.x(), rect.top()));
            return;
        }
    }

    if(!count_)
        return;

    const Point last_vertex = vertex_[count_ - 1];

    if ((last_vertex.x() < rect.right())
            && isBetweenY(rect, vertex_[0], last_vertex))
    {
        insertVertex(i + 1, Point(last_vertex.x(), rect.bottom()));
        insertVertex(i + 2, rect.rightBottom());
        insertVertex(i + 3, rect.rightTop());
        insertVertex(i + 4, Point(last_vertex.x(), rect.top()));
    }
}

void CommonData::insertTop(const Rect& rect)
{
    for (int i = 0; i < count_ - 1; i++) {
        Point v = vertex_[i];
        if (isBetweenX(rect, vertex_[i+1], vertex_[i])
                && (v.y() > rect.top())) {
            insertVertex(i + 1, Point(rect.right(), v.y()));
            insertVertex(i + 2, rect.rightTop());
            insertVertex(i + 3, rect.leftTop());
            insertVertex(i + 4, Point(rect.left(), v.y()));
            break;
        }
    }
}

int CommonData::isInPoly(const Point& pt) const
{
    const int point_x = pt.x();
    const int point_y = pt.y();
    int count = 0;
    for (uint i = 0; i < count_; i++) {
        const Point32 curr_point = vertex_[i];
        const Point32 next_point = vertex_[(i + 1) % count_];
        if (curr_point.x() == next_point.x()) {
            // on left side
            if (curr_point.x() > point_x)
                continue;

            int min_y = curr_point.y();
            int max_y = next_point.y();
            if(min_y > max_y)
                std::swap(min_y, max_y);

            if (min_y < point_y && point_y <= max_y) {
                if (curr_point.x() == point_x)
                    return true;

                count++;
            }
        } else {
            int min_x = curr_point.x();
            int max_x = next_point.x();
            if(min_x > max_x)
                std::swap(min_x, max_x);

            if (point_x >= min_x && point_x <= max_x) {
                if (curr_point.y() == point_y) {
                    return true;
                }
            }
        }
    }

    return count & 1;
}

int CommonData::isInPoly(const Point16& pt) const
{
    Point p32;
    p32 = pt;
    return isInPoly(p32);
}

bool CommonData::isRect() const
{
    if(count_ != 4)
        return false;

    if(vertex_[0].x() != vertex_[3].x())
        return false;
    if(vertex_[1].x() != vertex_[2].x())
        return false;
    if(vertex_[0].y() != vertex_[1].y())
        return false;
    if(vertex_[3].y() != vertex_[2].y())
        return false;
    if(vertex_[0].x() >= vertex_[1].x())
        return false;
    if(vertex_[0].y() >= vertex_[3].y())
        return false;

    return true;
}

void CommonData::moveVertexX(size_t pos, int dx)
{
    assert(pos < CPAGE_MAXCORNER);
    vertex_[pos].rx() += dx;
}

void CommonData::moveVertexY(size_t pos, int dy)
{
    assert(pos < CPAGE_MAXCORNER);
    vertex_[pos].ry() += dy;
}

int CommonData::number() const
{
    return number_;
}

void CommonData::rotateVertexesToIdeal(int skew)
{
    for(size_t i = 0; i < count_; i++)
        rotateVertexToIdeal<2048>(i, skew);
}

void CommonData::rotateVertexesToReal(int skew)
{
    for(size_t i = 0; i < count_; i++)
        rotateVertexToReal<2048>(i, skew);
}

void CommonData::setNumber(int n)
{
    number_ = n;
}

void CommonData::setRect(const Rect& r)
{
    vertex_[0] = r.leftTop();
    vertex_[1] = r.rightTop();
    vertex_[2] = r.rightBottom();
    vertex_[3] = r.leftBottom();
    count_ = 4;
}

void CommonData::setType(CDataType type)
{
    type_ = type;
}

void CommonData::setVertex(size_t pos, const Point& pt)
{
    assert(pos < CPAGE_MAXCORNER);
    vertex_[pos] = pt;
}

void CommonData::setVertexX(size_t pos, int x)
{
    assert(pos < CPAGE_MAXCORNER);
    vertex_[pos].setX(x);
}

void CommonData::setVertexY(size_t pos, int y)
{
    assert(pos < CPAGE_MAXCORNER);
    vertex_[pos].setY(y);
}

CDataType CommonData::type() const
{
    return type_;
}

Point& CommonData::vertexAt(size_t pos)
{
    assert(pos < CPAGE_MAXCORNER);
    return vertex_[pos];
}

const Point& CommonData::vertexAt(size_t pos) const
{
    assert(pos < CPAGE_MAXCORNER);
    return vertex_[pos];
}

int CommonData::vertexX(size_t pos) const
{
    return vertexAt(pos).x();
}

int CommonData::vertexY(size_t pos) const
{
    return vertexAt(pos).y();
}

size_t CommonData::vertexCount() const
{
    return count_;
}

}
}
