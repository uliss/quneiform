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

#ifndef RECT_H_
#define RECT_H_

#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "point.h"
#include "size.h"

namespace cf
{

struct RectCheckNormalized
{
        template<class T>
        static void check(const T& rect) {
            if (!rect.isValid())
                throw std::runtime_error("Rectangle is not normalized");
        }
};

struct RectCheckNone
{
        template<class T>
        static void check(const T&) {
        }
};

template<class T, class NormalizeChecker = RectCheckNone>
class RectImpl
{
    public:
        RectImpl(T left, T top, T width, T height) :
            pt0_(left, top), pt1_(left + width, top + height) {
            NormalizeChecker::check(*this);
        }

        RectImpl(const PointImpl<T>& pt, T width, T height) :
            pt0_(pt), pt1_(pt + PointImpl<T> (width, height)) {
            NormalizeChecker::check(*this);
        }

        RectImpl(const PointImpl<T>& pt0, const PointImpl<T>& pt1) :
            pt0_(pt0), pt1_(pt1) {
            NormalizeChecker::check(*this);
        }

        /**
         * Constructs rectangle at (0,0) with 0 width and height
         */
        RectImpl() {
        }

        void adjust(T dx0, T dy0, T dx1, T dy1) {
            pt0_.rx() += dx0;
            pt0_.ry() += dy0;
            pt1_.rx() += dx1;
            pt1_.ry() += dy1;
            NormalizeChecker::check(*this);
        }

        RectImpl adjusted(T dx0, T dy0, T dx1, T dy1) const {
            RectImpl tmp(*this);
            tmp.adjust(dx0, dy0, dx1, dy1);
            return tmp;
        }

        /**
         * Returns bottom rectangle y-coord
         */
        T bottom() const {
            return pt1_.y();
        }

        /**
         * Returns point that situated in the center of rectangle
         * @see centerX(), centerY()
         */
        PointImpl<T> center() const {
            return PointImpl<T> (centerX(), centerY());
        }

        /**
         * Returns x-coord of rectangle center
         * @see centerY(), center()
         */
        T centerX() const {
            return (left() + right()) / 2;
        }

        /**
         * Returns y-coord of rectangle center
         * @see centerX(), center()
         */
        T centerY() const {
            return (top() + bottom()) / 2;
        }

        /**
         * Checks if point with coords @b x and @b y is inside rectangle
         * @param x - coordinate of point
         * @param y - coordinate of point
         * @param proper - if set to @b true not takes into account rectangle borders
         * @return true if point is in rectangle
         */
        bool contains(T x, T y, bool proper) const {
            return proper ? (left() < x && top() < y && x < right() && y < bottom()) : (left() <= x
                    && top() <= y && x <= right() && y <= bottom());
        }

        bool contains(const PointImpl<T>& pt, bool proper = false) const {
            return contains(pt.x(), pt.y(), proper);
        }

        bool contains(const RectImpl& rect, bool proper = false) const {
            if (proper)
                return top() < rect.top() && left() < rect.left() && rect.bottom() < bottom()
                        && rect.right() < right();

            else
                return top() <= rect.top() && left() <= rect.left() && rect.bottom() <= bottom()
                        && rect.right() <= right();
        }

        /**
         * Returns rectangle diagonal
         */
        double diagonal() const {
            return ::sqrt(height() * height() + width() * width());
        }

        bool intersects(const RectImpl& r) const {
            if (left() > r.right() || right() < r.left() || top() > r.bottom() || bottom()
                    < r.top())
                return false;

            return true;
        }

        RectImpl intersected(const RectImpl& r) {
            return RectImpl(
                        PointImpl<T>(std::max(left(), r.left()),
                                     std::max(top(), r.top())),
                        PointImpl<T> (std::min(right(), r.right()),
                                      std::min(bottom(), r.bottom()))
                        );
        }

        bool isPositive() const {
            return pt0_.isPositive() && pt1_.isPositive();
        }

        bool isNull() const {
            return left() == std::numeric_limits<T>::max() &&
                    top() == std::numeric_limits<T>::max() &&
                    right() == std::numeric_limits<T>::min() &&
                    bottom() == std::numeric_limits<T>::min();
        }

        bool isValid() const {
            return left() <= right() && top() <= bottom();
        }

        T height() const {
            return pt1_.y() - pt0_.y();
        }

        void normalize() {
            if (pt0_.x() > pt1_.x())
                std::swap(pt0_.rx(), pt1_.rx());

            if (pt0_.y() > pt1_.y())
                std::swap(pt0_.ry(), pt1_.ry());
        }

        RectImpl normalized() const {
            RectImpl tmp(*this);
            tmp.normalize();
            return tmp;
        }

        T left() const {
            return pt0_.x();
        }

        PointImpl<T> leftBottom() const {
            return PointImpl<T> (left(), bottom());
        }

        PointImpl<T> leftTop() const {
            return pt0_;
        }

        template<class U>
        void moveBy(const PointImpl<U>& pt) {
            pt0_ += pt;
            pt1_ += pt;
        }

        void moveXTo(T x) {
            T w = width();
            pt0_.setX(x);
            setWidth(w);
        }

        void moveYTo(T y) {
            T h = height();
            pt0_.setY(y);
            setHeight(h);
        }

        void moveTo(T x, T y) {
            moveXTo(x);
            moveYTo(y);
        }

        template<class U>
        void moveTo(const PointImpl<U>& pt) {
            moveXTo(pt.x());
            moveYTo(pt.y());
        }

        template<class U>
        void operator=(const RectImpl<U>& rect) {
            pt0_ = rect.pt0();
            pt1_ = rect.pt1();
        }

        bool operator==(const RectImpl& r) const {
            return pt0_ == r.pt0_ && pt1_ == r.pt1_;
        }

        bool operator!=(const RectImpl& r) const {
            return !this->operator==(r);
        }

        void operator|=(const RectImpl& rect) {
            pt0_.rx() = std::min(left(), rect.left());
            pt0_.ry() = std::min(top(), rect.top());
            pt1_.rx() = std::max(right(), rect.right());
            pt1_.ry() = std::max(bottom(), rect.bottom());
        }

        T perimeter() const {
            return (width() + height()) * 2;
        }

        PointImpl<T>& pt0() {
            return pt0_;
        }

        const PointImpl<T>& pt0() const {
            return pt0_;
        }

        PointImpl<T>& pt1() {
            return pt1_;
        }

        const PointImpl<T>& pt1() const {
            return pt1_;
        }

        T right() const {
            return pt1_.x();
        }

        PointImpl<T> rightBottom() const {
            return pt1_;
        }

        PointImpl<T> rightTop() const {
            return PointImpl<T> (right(), top());
        }

        T& rbottom() {
            return pt1_.ry();
        }

        T& rleft() {
            return pt0_.rx();
        }

        T& rright() {
            return pt1_.rx();
        }

        T& rtop() {
            return pt0_.ry();
        }

        void set(const PointImpl<T>& pos, T width, T height) {
            pt0_ = pos;
            pt1_.set(pos.x() + width, pos.y() + height);
            NormalizeChecker::check(*this);
        }

        void set(const PointImpl<T>& p0, const PointImpl<T>& p1) {
            pt0_ = p0;
            pt1_ = p1;
            NormalizeChecker::check(*this);
        }

        void setBottom(T bottom) {
            pt1_.setY(bottom);
            NormalizeChecker::check(*this);
        }

        void setLeftBottom(const PointImpl<T>& pt) {
            setLeft(pt.x());
            setBottom(pt.y());
        }

        void setLeftBottom(T x, T y) {
            setLeft(x);
            setBottom(y);
        }

        void setLeftTop(const PointImpl<T>& pt) {
            pt0_ = pt;
        }

        void setLeftTop(T x, T y) {
            pt0_.set(x, y);
        }

        void setHeight(T t) {
            pt1_.setY(pt0_.y() + t);
            NormalizeChecker::check(*this);
        }

        void setLeft(T left) {
            pt0_.setX(left);
            NormalizeChecker::check(*this);
        }

        void setRight(T right) {
            pt1_.setX(right);
            NormalizeChecker::check(*this);
        }

        void setRightBottom(const PointImpl<T>& pt) {
            pt1_ = pt;
            NormalizeChecker::check(*this);
        }

        void setRightBottom(T x, T y) {
            pt1_.set(x, y);
            NormalizeChecker::check(*this);
        }

        void setRightTop(const PointImpl<T>& pt) {
            setRight(pt.x());
            setTop(pt.y());
        }

        void setRightTop(T x, T y) {
            setRight(x);
            setTop(y);
        }

        void setSize(const SizeImpl<T>& size) {
            setWidth(size.width());
            setHeight(size.height());
        }

        void setTop(T top) {
            pt0_.setY(top);
            NormalizeChecker::check(*this);
        }

        void setWidth(T t) {
            pt1_.setX(pt0_.x() + t);
            NormalizeChecker::check(*this);
        }

        RectImpl& scale(double factor) {
            setSize(size().scale(factor));
            return *this;
        }

        SizeImpl<T> size() const {
            return SizeImpl<T> (width(), height());
        }

        RectImpl& translate(const PointImpl<T>& pt) {
            pt0_ += pt;
            pt1_ += pt;
            return *this;
        }

        RectImpl& translate(T dx, T dy) {
            return translate(PointImpl<T> (dx, dy));
        }

        RectImpl translated(const PointImpl<T>& pt) const {
            return RectImpl(pt0_ + pt, pt1_ + pt);
        }

        RectImpl& translateX(T offset) {
            pt0_.rx() += offset;
            pt1_.rx() += offset;
            return *this;
        }

        RectImpl& translateY(T offset) {
            pt0_.ry() += offset;
            pt1_.ry() += offset;
            return *this;
        }

        T top() const {
            return pt0_.y();
        }

        RectImpl united(const RectImpl& r) {
            return RectImpl(PointImpl<T> (std::min(left(), r.left()), std::min(top(), r.top())),
                    PointImpl<T> (std::max(right(), r.right()), std::max(bottom(), r.bottom())));
        }

        T width() const {
            return pt1_.x() - pt0_.x();
        }

        T x() const {
            return pt0_.x();
        }

        T y() const {
            return pt0_.y();
        }
    public:
        static RectImpl<T> null() {
            RectImpl n;
            n.pt0_.rx() = std::numeric_limits<T>::max();
            n.pt0_.ry() = std::numeric_limits<T>::max();
            n.pt1_.rx() = std::numeric_limits<T>::min();
            n.pt1_.ry() = std::numeric_limits<T>::min();
            return n;
        }
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("pt0", pt0_);
            ar & make_nvp("pt1", pt1_);
        }
#endif
        PointImpl<T> pt0_, pt1_;
};

typedef RectImpl<int, RectCheckNone> Rect;
typedef RectImpl<short, RectCheckNone> Rect16;

}

#ifdef CF_SERIALIZE
BOOST_IS_BITWISE_SERIALIZABLE(cf::Rect)
BOOST_IS_BITWISE_SERIALIZABLE(cf::Rect16)
#endif

#endif /* RECT_H_ */
