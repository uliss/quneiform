/*
 * rect.h
 *
 *  Created on: 09.10.2009
 *      Author: uliss
 */

#ifndef RECT_H_
#define RECT_H_

#include <algorithm>

#include "point.h"

namespace CIF {

template<class T>
class RectImpl {
public:
	RectImpl(const PointImpl<T>& pt, T height, T width) :
		pt0_(pt), pt1_(pt + PointImpl<T> (height, width)) {
	}

	RectImpl(const PointImpl<T>& pt0, const PointImpl<T>& pt1) :
		pt0_(pt0), pt1_(pt1) {
	}

	RectImpl() {
	}

	T height() const {
		return pt1_.y() - pt0_.y();
	}

	void normalize() {
		if (pt0_.x() > pt1_.x())
			std::swap(pt0_, pt1_);
		if (pt0_.y() > pt1_.y())
			std::swap(pt0_, pt1_);
	}

	T bottom() const {
		return pt1_.y();
	}

	T left() const {
		return pt0_.x();
	}

	bool operator==(const RectImpl& r) const {
		return pt0_ == r.pt0_ && pt1_ == r.pt1_;
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

	void setHeight(T t) {
		pt1_.setX(pt0_.x() + t);
	}

	void setWidth(T t) {
		pt1_.setY(pt0_.y() + t);
	}

	RectImpl& transposeX(T offset) {
		pt0_.x() += offset;
		pt1_.y() += offset;
		return *this;
	}

	T top() const {
		return pt0_.y();
	}

	T width() const {
		return pt1_.x() - pt0_.x();
	}
private:
	PointImpl<T> pt0_, pt1_;
};

typedef RectImpl<int> Rect;

}

#endif /* RECT_H_ */
