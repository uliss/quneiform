/*
 * memorybuffer.h
 *
 *  Created on: 09.10.2009
 *      Author: uliss
 */

#ifndef MEMORYBUFFER_H_
#define MEMORYBUFFER_H_

#include <cstring>
#include <cassert>

namespace CIF {

template<class T, size_t N>
class FixedBuffer {
public:
	FixedBuffer(bool initZero = false) {
		if (initZero)
			clear();
	}

	T * begin() {
		return buf_;
	}

	T * end() {
		return &buf_[N];
	}

	void clear() {
		::memset(buf_, 0, sizeof(T) * N);
	}

	T& operator[](size_t index) {
		assert(index < N);
		return buf_[index];
	}

	const T& operator[](size_t index) const {
		assert(index < N);
		return buf_[index];
	}

	size_t size() const {
		return N;
	}
private:
	T buf_[N + 1];
};
}

#endif /* MEMORYBUFFER_H_ */
