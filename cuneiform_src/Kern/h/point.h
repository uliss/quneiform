/*
 * point.h
 *
 *  Created on: 29.09.2009
 *      Author: uliss
 */

#ifndef POINT_H_
#define POINT_H_

namespace CIF {

template<class T>
class PointImpl {
public:
	PointImpl(T x, T y) :
		x_(x), y_(y) {
	}

	PointImpl() :
		x_(0), y_(0) {
	}

	T& x() {
		return x_;
	}

	const T& x() const {
		return x_;
	}

	T& y() {
		return y_;
	}

	const T& y() const {
		return y_;
	}
public:
	static T dist_x(const PointImpl& p0, const PointImpl& p1) {
		return p0.x_ - p1.x_;
	}

	static T dist_y(const PointImpl& p0, const PointImpl& p1) {
		return p0.y_ - p1.y_;
	}

	static bool higher(const PointImpl& p0, const PointImpl& p1) {
		return p0.y_ > p1.y_;
	}

	static bool lower(const PointImpl& p0, const PointImpl& p1) {
		return p0.y_ < p1.y_;
	}
private:
	T x_;
	T y_;
};

typedef PointImpl<int> Point;
typedef PointImpl<char> Point8;
}

#endif /* POINT_H_ */
