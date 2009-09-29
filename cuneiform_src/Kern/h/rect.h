/*
 * rect.h
 *
 *  Created on: 29.09.2009
 *      Author: uliss
 */

#ifndef RECT_H_
#define RECT_H_

#include "point.h"

namespace CIF {

template<class T>
class RectImpl {
public:
	RectImpl() {
	}

	RectImpl(T left, T top, T right, T bottom) :
		p0_(left, top), p1_(right, bottom_) {
	}

	RectImpl(const typename Point<T>& p0, const typename Point<T>& p1) :
	p0_(p0), p1_(p1) {
	}

	T& left() {
		return p0_.x();
	}

	T& right() {
		return p1_.x();
	}

	T& top() {
		return p0_.y();
	}

	T& bottom() {
		return p1_.y();
	}
private:
	typename Point<T> p0_;
	typename Point<T> p1_;
};
}

#endif /* RECT_H_ */
