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
