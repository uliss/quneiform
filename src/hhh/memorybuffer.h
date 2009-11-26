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

namespace CIF
{

template<class T>
struct InitNone {
    static void init(void * mem, size_t size) {
    }
};

template<class T>
struct InitZero {
    static void init(void * mem, size_t size) {
        ::memset(mem, 0, size);
    }
};

template < class T, template<class > class InitStrategy = InitNone >
class AutoBuffer
{
        AutoBuffer(const AutoBuffer&);
        void operator=(const AutoBuffer&);
    public:
        AutoBuffer(size_t size) :
                buf_(0), size_(size) {
            buf_ = new T[size_ + 1];
            InitStrategy<T>::init(buf_, size_ * sizeof(T));
        }

        ~AutoBuffer() {
            delete[] buf_;
        }

        T * begin() {
            return buf_;
        }

        T * end() {
            return &buf_[size_];
        }

        T& operator[](size_t index) {
            assert(index < size_);
            return buf_[index];
        }

        const T& operator[](size_t index) const {
            assert(index < size_);
            return buf_[index];
        }

        size_t size() const {
            return size_;
        }
    private:
        T * buf_;
        size_t size_;
};

template<class T, size_t N>
class FixedBuffer
{
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
