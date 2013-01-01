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

namespace cf {

namespace cpage {

CommonData::CommonData() :
    type_(0),
    count(0),
    number_(0),
    flags_(0)
{
}

void CommonData::addVertex(const Point &pt)
{
    Vertex[count] = pt;
    count++;
}

void CommonData::copyVertexes(const Picture& pict)
{
    for(size_t i = 0; i < pict.cornerCount(); i++)
        addVertex(pict.cornerAt(i));
}

int CommonData::calcHeight() const
{
    if(!count)
        return 0;

    int min_top = Vertex[0].y();
    int max_bottom = min_top;

    for(int i = 1; i < count; i++) {
        Point p = Vertex[i];
        min_top = std::min(min_top, p.y());
        max_bottom = std::max(max_bottom, p.y());
    }

    return max_bottom - min_top;
}

void CommonData::crossBy0(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 2; i++) {
        if ((Vertex[i].y() < rect.Vertex[0].y())
                && (Vertex[i + 1].x() > rect.Vertex[0].x())
                && (Vertex[i + 1].y() > rect.Vertex[0].y())
                && (Vertex[i + 2].x() < rect.Vertex[0].x())) {
            point.ry() = rect.Vertex[0].y();
            point.rx() = Vertex[i].x();
            insertVertex(i + 1, point);
            Vertex[i + 2] = rect.Vertex[0];
            point.ry() = Vertex[i + 3].y();
            point.rx() = rect.Vertex[0].x();
            insertVertex(i + 3, point);
            break;
        }
    }
}

void CommonData::crossBy1(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 2; i++) { //STEPA_AM |
        if ((Vertex[i].x() > rect.Vertex[1].x())
                && (Vertex[i + 1].x() < rect.Vertex[1].x())
                && (Vertex[i + 1].y() > rect.Vertex[1].y())
                && (Vertex[i + 2].y() < rect.Vertex[1].y())) {
            point.rx() = rect.Vertex[1].x();
            point.ry() = Vertex[i].y();
            insertVertex(i + 1, point);
            Vertex[i + 2] = rect.Vertex[1];
            point.rx() = Vertex[i + 3].x();
            point.ry() = rect.Vertex[1].y();
            insertVertex(i + 3, point);
            return;
        }
    }

    if ((Vertex[count - 2].x() > rect.Vertex[1].x())
            && (Vertex[count - 1].x() < rect.Vertex[1].x())
            && (Vertex[count - 1].y() > rect.Vertex[1].y())
            && (Vertex[0].y() < rect.Vertex[1].y())) {
        point.rx() = rect.Vertex[1].x();
        point.ry() = Vertex[count - 2].y();
        insertVertex(count - 1, point);
        insertVertex(count - 1, rect.Vertex[1]);
        point.rx() = Vertex[0].x();
        point.ry() = rect.Vertex[1].y();
        Vertex[count - 1] = point;
    }
}

void CommonData::crossBy2(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 2; i++) {
        if ((Vertex[i].y() > rect.Vertex[2].y())
                && (Vertex[i + 1].x() < rect.Vertex[2].x())
                && (Vertex[i + 1].y() < rect.Vertex[2].y())
                && (Vertex[i + 2].x() > rect.Vertex[2].x())) {
            point.ry() = rect.Vertex[2].y();
            point.rx() = Vertex[i].x();
            insertVertex(i + 1, point);
            Vertex[i + 2] = rect.Vertex[2];
            point.ry() = Vertex[i + 3].y();
            point.rx() = rect.Vertex[2].x();
            insertVertex(i + 3, point);
            return;
        }
    }

    if ((Vertex[count - 1].y() > rect.Vertex[2].y())
            && (Vertex[0].x() < rect.Vertex[2].x())
            && (Vertex[0].y() < rect.Vertex[2].y())
            && (Vertex[1].x() > rect.Vertex[2].x())) {
        //Special case - use with cautions :)
        point.ry() = Vertex[0].y();
        point.rx() = rect.Vertex[2].x();
        insertVertex(1, point);
        point.ry() = rect.Vertex[2].y();
        point.rx() = Vertex[0].x();
        insertVertex(1, rect.Vertex[2]);
        Vertex[0] = point;
    }
}

void CommonData::crossBy3(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 2; i++) {
        if ((Vertex[i].x() < rect.Vertex[3].x())
                && (Vertex[i + 1].x() > rect.Vertex[3].x())
                && (Vertex[i + 1].y() < rect.Vertex[3].y())
                && (Vertex[i + 2].y() > rect.Vertex[3].y())) {
            point.rx() = rect.Vertex[3].x();
            point.ry() = Vertex[i].y();
            insertVertex(i + 1, point);
            Vertex[i + 2] = rect.Vertex[3];
            point.rx() = Vertex[i + 3].x();
            point.ry() = rect.Vertex[3].y();
            insertVertex(i + 3, point);
            break;
        }
    }
}

void CommonData::insertVertex(size_t pos, const Point &p)
{
    count++;
    assert(count > 0);

    for (size_t i = count - 1; i >= pos; i--)
        Vertex[i] = Vertex[i - 1];

    Vertex[pos] = p;
}

void CommonData::insertBottom(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 1; i++) {
        if ((Vertex[i].x() < rect.Vertex[3].x())
                && (Vertex[i].y() < rect.Vertex[3].y())
                && (Vertex[i + 1].x() > rect.Vertex[2].x())) {
            point.rx() = rect.Vertex[3].x();
            point.ry() = Vertex[i].y();
            insertVertex(i + 1, point);
            insertVertex(i + 2, rect.Vertex[3]);
            insertVertex(i + 3, rect.Vertex[2]);
            point.rx() = rect.Vertex[2].x();
            insertVertex(i + 4, point);
            break;
        }
    }
}

void CommonData::insertLeft(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 1; i++) {
        if ((Vertex[i].x() > rect.Vertex[0].x())
                && (Vertex[i].y() < rect.Vertex[0].y())
                && (Vertex[i + 1].y() > rect.Vertex[3].y())) {
            point.rx() = Vertex[i].x();
            point.ry() = rect.Vertex[0].y();
            insertVertex(i + 1, point);
            insertVertex(i + 2, rect.Vertex[0]);
            insertVertex(i + 3, rect.Vertex[3]);
            point.ry() = rect.Vertex[3].y();
            insertVertex(i + 4, point);
            break;
        }
    }

}

void CommonData::insertRight(const CommonData& rect)
{
    Point point;
    int i = 0;

    for (i = 0; i < count - 1; i++) {
        if ((Vertex[i].x() < rect.Vertex[2].x())
                && (Vertex[i].y() > rect.Vertex[2].y())
                && (Vertex[i + 1].y() < rect.Vertex[1].y())) {
            point.rx() = Vertex[i].x();
            point.ry() = rect.Vertex[2].y();
            insertVertex(i + 1, point);
            insertVertex(i + 2, rect.Vertex[2]);
            insertVertex(i + 3, rect.Vertex[1]);
            point.ry() = rect.Vertex[1].y();
            insertVertex(i + 4, point);
            return;
        }
    }

    if ((Vertex[count - 1].x() < rect.Vertex[2].x())
            && (Vertex[count - 1].y() > rect.Vertex[2].y())
            && (Vertex[0].y() < rect.Vertex[1].y()))
    {
        point.rx() = Vertex[count - 1].x();
        point.ry() = rect.Vertex[2].y();
        insertVertex(i + 1, point);
        insertVertex(i + 2, rect.Vertex[2]);
        insertVertex(i + 3, rect.Vertex[1]);
        point.ry() = rect.Vertex[1].y();
        insertVertex(i + 4, point);
    }
}

void CommonData::insertTop(const CommonData& rect)
{
    Point point;

    for (int i = 0; i < count - 1; i++) {
        if ((Vertex[i].x() > rect.Vertex[1].x())
                && (Vertex[i].y() > rect.Vertex[1].y())
                && (Vertex[i + 1].x() < rect.Vertex[0].x())) {
            point.rx() = rect.Vertex[1].x();
            point.ry() = Vertex[i].y();
            insertVertex(i + 1, point);
            insertVertex(i + 2, rect.Vertex[1]);
            insertVertex(i + 3, rect.Vertex[0]);
            point.rx() = rect.Vertex[0].x();
            insertVertex(i + 4, point);
            break;
        }
    }
}

int CommonData::isInPoly(const Point& pt) const
{
    int y, ind;
    int Count = 0;

    for (int i = 0; i < count; i++) {
        int next = (i + 1) % count;

        if(Vertex[i].y() == Vertex[next].y())
            continue;

        if(Vertex[i].y() > pt.y() && Vertex[next].y() > pt.y())
            continue;

        if(Vertex[i].y() < pt.y() && Vertex[next].y() < pt.y())
            continue;

        y = Vertex[i].y();
        ind = i;

        if (Vertex[next].y() > Vertex[i].y()) {
            y = Vertex[next].y();
            ind = next;
        }

        if ((y == pt.y()) && (Vertex[ind].x() >= pt.x()))
            Count++;
        else if (std::min(Vertex[i].y(), Vertex[next].y()) == pt.y())
            continue;
        else {
            double t = ((double) (pt.y() - Vertex[i].y()) / ((double) (Vertex[next].y()
                    - (double) Vertex[i].y())));

            if (t > 0 && t < 1 && (double) Vertex[i].x() + t
                    * ((double) Vertex[next].x() - (double) Vertex[i].x())
                    >= (double) pt.x())
                Count++;
        }
    }

    return Count & 1;
}

int CommonData::isInPoly(const Point16& pt) const
{
    Point p32;
    p32 = pt;
    return isInPoly(p32);
}

bool CommonData::isRect() const
{
    if(count != 4)
        return false;

    if(Vertex[0].x() != Vertex[3].x())
        return false;
    if(Vertex[1].x() != Vertex[2].x())
        return false;
    if(Vertex[0].y() != Vertex[1].y())
        return false;
    if(Vertex[3].y() != Vertex[2].y())
        return false;
    if(Vertex[0].x() >= Vertex[1].x())
        return false;
    if(Vertex[0].y() >= Vertex[3].y())
        return false;

    return true;
}

void CommonData::moveVertexX(size_t pos, int dx)
{
    assert(pos < CPAGE_MAXCORNER);
    Vertex[pos].rx() + dx;
}

void CommonData::moveVertexY(size_t pos, int dy)
{
    assert(pos < CPAGE_MAXCORNER);
    Vertex[pos].ry() + dy;
}

int CommonData::number() const
{
    return number_;
}

void CommonData::rotateVertexesToIdeal(int skew)
{
    for(int i = 0; i < count; i++)
        rotateVertexToIdeal<2048>(i, skew);
}

void CommonData::setNumber(int n)
{
    number_ = n;
}

void CommonData::setRect(const Rect& r)
{
    Vertex[0] = r.leftTop();
    Vertex[1] = r.rightTop();
    Vertex[2] = r.rightBottom();
    Vertex[3] = r.leftBottom();
    count = 4;
}

void CommonData::setType(CDataType type)
{
    type_ = type;
}

void CommonData::setVertex(size_t pos, const Point& pt)
{
    assert(pos < CPAGE_MAXCORNER);
    Vertex[pos] = pt;
}

void CommonData::setVertexX(size_t pos, int x)
{
    assert(pos < CPAGE_MAXCORNER);
    Vertex[pos].setX(x);
}

void CommonData::setVertexY(size_t pos, int y)
{
    assert(pos < CPAGE_MAXCORNER);
    Vertex[pos].setY(y);
}

CDataType CommonData::type() const
{
    return type_;
}

Point& CommonData::vertexAt(size_t pos)
{
    assert(pos < CPAGE_MAXCORNER);
    return Vertex[pos];
}

const Point& CommonData::vertexAt(size_t pos) const
{
    assert(pos < CPAGE_MAXCORNER);
    return Vertex[pos];
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
    return count;
}

}
}
