/*
 * membuffer.h
 *
 *  Created on: 29.09.2009
 *      Author: uliss
 */

#ifndef MEMBUFFER_H_
#define MEMBUFFER_H_

#include <cstddef>
#include <cassert>
#include <cstring>

namespace CIF {

template<class T>
class AllocatorNew {
public:
	static T * alloc(size_t size) {
		return new T[size];
	}

	static void free(T * b) {
		delete[] b;
	}
private:
	AllocatorNew();
};

template<class T>
class AllocatorNewZero {
public:
	static T * alloc(size_t size) {
		T * mem = AllocatorNew<T>::alloc(size);
		::memset(mem, 0, sizeof(T) * size);
		return mem;
	}

	static void free(T * b) {
		AllocatorNew<T>::free(b);
	}
private:
	AllocatorNewZero();
};

template<size_t SIZE, template<class > class allocator = AllocatorNew>
class MemBuffer {
public:
	MemBuffer() {
		buffer_ = allocator<unsigned char>::alloc(SIZE);
	}

	~MemBuffer() {
		allocator<unsigned char>::free(buffer_);
	}

	unsigned char * get() {
		return buffer_;
	}

	size_t size() const {
		return SIZE;
	}
private:
	MemBuffer(const MemBuffer&);
	MemBuffer& operator=(const MemBuffer&);
private:
	unsigned char * buffer_;
};

template<class T, template<class > class allocator = AllocatorNew>
class AutoArray {
public:
	AutoArray(size_t size) :
		size_(size), array_(0) {
		array_ = allocator<T>::alloc(size);
	}

	~AutoArray() {
		allocator<T>::free(array_);
	}

	T& operator[](size_t n) {
		assert(n < size_);
		return array_[n];
	}

	const T& operator[](size_t n) const {
		assert(n < size_);
		return array_[n];
	}

	T * get() {
		return array_;
	}
private:
	const size_t size_;
	T * array_;
private:
	AutoArray(const AutoArray&);
	AutoArray& operator=(const AutoArray&);
};

template<class T>
class AutoArrayZ: public AutoArray<T, AllocatorNewZero> {
public:
	AutoArrayZ(size_t size) :
		AutoArray<T, AllocatorNewZero> (size) {
	}
};

}

#endif /* MEMBUFFER_H_ */
