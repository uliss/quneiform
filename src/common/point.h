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

#ifndef POINT_H_
#define POINT_H_

/**
 * Prevents conflicting with <windows.h> min/max macroses
 */
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <limits>
#include <cassert>
#include "cttypes.h"

namespace CIF
{

struct OverflowCheckNumeric {
    template<class T>
    static void check(long long int value) {
        assert(value <= std::numeric_limits<T>::max());
    }
};

struct UnderflowCheckNumeric {
    template<class T>
    static void check(long long int value) {
        assert(value >= std::numeric_limits<T>::min());
    }
};

struct OverflowCheckNone {
    template<class T>
    static void check(long long int) {
    }
};

struct UnderflowCheckNone {
    template<class T>
    static void check(long long int) {
    }
};

template < class T, class OverflowChecker = OverflowCheckNumeric, class UnderflowChecker = UnderflowCheckNumeric >
class PointImpl
{
    public:
        PointImpl(T x, T y) :
                x_(x), y_(y) {
        }

        PointImpl() :
                x_(0), y_(0) {
        }

        PointImpl& deskew(int skew1024) {
            // TODO check not only 32 code
            long phi_sq = skew1024 * skew1024;
            long dx = (skew1024 * y_ + 0x200) >> 10;
            long dy = (skew1024 * x_ + 0x200) >> 10;
            long ddx = (phi_sq * x_ + 0x100000) >> 21;
            long ddy = (phi_sq * y_ + 0x100000) >> 21;
            x_ -= dx;
            x_ -= ddx;
            y_ += dy;
            y_ -= ddy;
            return *this;;
        }

        PointImpl& deskew_rel(int skew1024, const PointImpl& rel) {
            x_ -= rel.x_;
            y_ -= rel.y_;
            deskew(skew1024);
            x_ += rel.x_;
            y_ += rel.y_;
            return *this;
        }

        bool isNegative() const {
            return x_ < 0 && y_ < 0;
        }

        bool operator==(const PointImpl& pt) const {
            return x_ == pt.x_ && y_ == pt.y_;
        }

        bool operator!=(const PointImpl& pt) const {
            return x_ != pt.x_ || y_ != pt.y_;
        }

        PointImpl operator+(const PointImpl& pt) const {
            OverflowChecker::template check<T>(x_ + pt.x_);
            OverflowChecker::template check<T>(y_ + pt.y_);
            return PointImpl(x_ + pt.x_, y_ + pt.y_);
        }

        PointImpl operator-(const PointImpl& pt) const {
            UnderflowChecker::template check<T>(x_ - pt.x_);
            UnderflowChecker::template check<T>(y_ - pt.y_);
            return PointImpl(x_ - pt.x_, y_ - pt.y_);
        }

        template<class U>
        PointImpl& operator+=(const PointImpl<U>& pt) {
            OverflowChecker::template check<T>(x_ + pt.x());
            OverflowChecker::template check<T>(y_ + pt.y());
            x_ += pt.x();
            y_ += pt.y();
            return *this;
        }

        template<class U>
        PointImpl& operator-=(const PointImpl<U>& pt) {
            UnderflowChecker::template check<T>(x_ - pt.x());
            UnderflowChecker::template check<T>(y_ - pt.y());
            x_ -= pt.x();
            y_ -= pt.y();
            return *this;
        }

        PointImpl& operator+=(T offset) {
            OverflowChecker::template check<T>(x_ + offset);
            OverflowChecker::template check<T>(y_ + offset);
            x_ += offset;
            y_ += offset;
            return *this;
        }

        PointImpl& operator-=(T offset) {
            UnderflowChecker::template check<T>(x_ - offset);
            UnderflowChecker::template check<T>(y_ - offset);
            x_ -= offset;
            y_ -= offset;
            return *this;
        }

        void operator=(const PointImpl<T>& pt) {
            x_ = pt.x_;
            y_ = pt.y_;
        }

        template<class U>
        void operator=(const PointImpl<U>& pt) {
            OverflowChecker::template check<T>(pt.x());
            OverflowChecker::template check<T>(pt.y());
            x_ = pt.x();
            y_ = pt.y();
        }

        void set(T x, T y) {
            x_ = x;
            y_ = y;
        }

        void setX(T x) {
            x_ = x;
        }

        void setY(T y) {
            y_ = y;
        }

        T& rx() {
            return x_;
        }

        T& ry() {
            return y_;
        }

        T x() const {
            return x_;
        }

        T y() const {
            return y_;
        }
    private:
        T x_, y_;
};

template<class T>
T PointXDelta(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    return p0.x() - p1.x();
}

template<class T>
T PointYDelta(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    return p0.y() - p1.y();
}

template<class T>
T PointXDistance(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    T res = p0.x() - p1.x();
    return res > 0 ? res : -res;
}

template<class T>
T PointYDistance(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    T res = p0.y() - p1.y();
    return res > 0 ? res : -res;
}

template<class T>
PointImpl<T> highest(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    return p0.y() < p1.y() ? p0 : p1;
}

template<class T>
PointImpl<T> leftmost(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    return p0.x() < p1.x() ? p0 : p1;
}

template<class T>
PointImpl<T> lowest(const PointImpl<T>& p0, const PointImpl<T>& p1)
{
    return p0.y() > p1.y() ? p0 : p1;
}

template<class T>
PointImpl<T> rightmost(PointImpl<T>& p0, PointImpl<T>& p1)
{
    return p0.x() > p1.x() ? p0 : p1;
}

typedef PointImpl<int32_t, OverflowCheckNumeric, UnderflowCheckNumeric> Point;
typedef PointImpl<int16_t, OverflowCheckNumeric, UnderflowCheckNumeric> Point16;

}

#endif
