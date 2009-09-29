/*
 * membuffer.h
 *
 *  Created on: 29.09.2009
 *      Author: uliss
 */

#ifndef MEMBUFFER_H_
#define MEMBUFFER_H_

#include <cstddef>

namespace CIF {

template<size_t SZ>
class MemBufferNew {
public:
	static unsigned char * alloc() {
		return new unsigned char[SZ];
	}

	static void free(unsigned char * b) {
		delete[] b;
	}
};

template<size_t SZ>
class MemBufferStatic {
public:
	static unsigned char * alloc() {
		static unsigned char buffer[SZ];
		return &buffer;
	}

	static void free(unsigned char *) {
	}
};

template<size_t SIZE, template<size_t> class allocator = MemBufferNew>
class MemBuffer {
public:
	MemBuffer() {
		buffer_ = allocator<SIZE>::alloc();
	}

	~MemBuffer() {
		allocator<SIZE>::free(buffer_);
	}

	unsigned char * get() {
		return buffer_;
	}

	size_t size() const {
		return SIZE;
	}
private:
	unsigned char * buffer_;
};
}

#endif /* MEMBUFFER_H_ */
