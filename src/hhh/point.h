/*
 * point.h
 *
 *  Created on: 09.10.2009
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

	bool higher(const PointImpl& pt) {
		return y_ > pt.y_;
	}

	bool lower(const PointImpl& pt) {
		return y_ < pt.y_;
	}

	bool operator==(const PointImpl& pt) const {
		return x_ == pt.x_ && y_ == pt.y_;
	}

	PointImpl operator+(const PointImpl& pt) const {
		return PointImpl(x_ + pt.x_, y_ + pt.y_);
	}

	PointImpl operator-(const PointImpl& pt) const {
		return PointImpl(x_ - pt.x_, y_ - pt.y_);
	}

	PointImpl& operator+=(const PointImpl& pt) {
		x_ += pt.x_;
		y_ += pt.y_;
		return *this;
	}

	PointImpl& operator-=(const PointImpl& pt) {
		x_ -= pt.x_;
		y_ -= pt.y_;
		return *this;
	}

	PointImpl& operator+=(T offset) {
		x_ += offset;
		y_ += offset;
		return *this;
	}

	PointImpl& operator-=(T offset) {
		x_ -= offset;
		y_ -= offset;
		return *this;
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

	const T& x() const {
		return x_;
	}

	T& y() {
		return y_;
	}

	const T& y() const {
		return y_;
	}
private:
	T x_, y_;
};

typedef PointImpl<int> Point;

}

#endif /* POINT_H_ */
