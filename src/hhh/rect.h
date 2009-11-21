/*
 * rect.h
 *
 *  Created on: 09.10.2009
 *      Author: uliss
 */

#ifndef RECT_H_
#define RECT_H_

#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "point.h"
#include "size.h"

namespace CIF {

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
    RectImpl(const PointImpl<T>& pt, T height, T width) :
        pt0_(pt), pt1_(pt + PointImpl<T> (height, width)) {
        NormalizeChecker::check(*this);
    }

    RectImpl(const PointImpl<T>& pt0, const PointImpl<T>& pt1) :
        pt0_(pt0), pt1_(pt1) {
        NormalizeChecker::check(*this);
    }

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

    T bottom() const {
        return pt1_.y();
    }

    PointImpl<T> center() const {
        return PointImpl<T> (centerX(), centerY());
    }

    T centerX() const {
        return pt0_.x() + width() / 2;
    }

    T centerY() const {
        return pt0_.y() + height() / 2;
    }

    bool contains(T x, T y, bool proper) {
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

    double diagonal() const {
        return ::sqrt(height() * height() + width() * width());
    }

    bool isPositive() const {
        return pt0_.isPositive() && pt1_.isPositive();
    }

    bool isValid() const {
        return left() < right() && top() < bottom();
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

    void set(const PointImpl<T>& pos, T width, T height) {
        pt0_ = pos;
        pt1_.set(pos.x() + width, pos.y() + height);
        NormalizeChecker::check(*this);
    }

    void setBottom(T bottom) {
        pt1_.setY(bottom);
        NormalizeChecker::check(*this);
    }

    void setBottomLeft(const PointImpl<T>& pt) {
        setLeft(pt.x());
        setBottom(pt.y());
    }

    void setBottomRight(const PointImpl<T>& pt) {
        pt1_ = pt;
        NormalizeChecker::check(*this);
    }

    void setHeight(T t) {
        pt1_.setX(pt0_.x() + t);
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

    void setSize(const SizeImpl<T>& size) {
        setWidth(size.width());
        setHeight(size.height());
    }

    void setTop(T top) {
        pt0_.setY(top);
        NormalizeChecker::check(*this);
    }

    void setTopLeft(const PointImpl<T>& pt) {
        pt0_ = pt;
    }

    void setTopRight(const PointImpl<T>& pt) {
        setRight(pt.x());
        setTop(pt.y());
    }

    void setWidth(T t) {
        pt1_.setY(pt0_.y() + t);
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

    T width() const {
        return pt1_.x() - pt0_.x();
    }

    T x() const {
        return pt0_.x();
    }

    T y() const {
        return pt0_.y();
    }
private:
    PointImpl<T> pt0_, pt1_;
};

typedef RectImpl<int, RectCheckNone> Rect;

}

#endif /* RECT_H_ */
