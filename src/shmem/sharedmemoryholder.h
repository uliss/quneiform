/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef SHAREDMEMORYHOLDER_H
#define SHAREDMEMORYHOLDER_H

#include <cstddef>
#include <string>
#include <stdexcept>

#include "globus.h"

namespace cf {

class SharedMemoryHolderPrivate;

class CLA_EXPO SharedMemoryHolder
{
public:
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg) : std::runtime_error(msg) {}
    };

    class LowSharedMemoryException : public Exception {
    public:
        LowSharedMemoryException(const std::string& msg, size_t required, size_t current) :
            Exception(msg),
            required_(required),
            current_(current)
        {}
        size_t current() const { return current_; }
        size_t required() const { return required_; }
    private:
        size_t required_;
        size_t current_;
    };
public:
    SharedMemoryHolder(bool owner = false);
    ~SharedMemoryHolder();

    /**
      * Attaches to shared memory with given key
      * @param key - shared memory key
      * @throw Exception on error
      * @see detach()
      */
    void attach(const std::string& key, size_t size);

    /**
      * Creates shared memory and attaches to it
      * @throw Exception on error
      * @see remove()
      */
    void create(const std::string& key, size_t size);

    /**
      * Detaches from shared memory
      * @see attach()
      */
    void detach();

    /**
      * Checks if memory attached
      */
    bool isAttached() const;

    /**
      * Returns pointer shared memory
      * @see size()
      */
    void * get();

    /**
      * Removes shared memory
      * @see create()
      * @throw Exception on error
      */
    void remove();

    /**
      * Returns shared memory size
      * @see get()
      */
    size_t size() const;

    template<class T>
    T * toObject(ptrdiff_t off) {
        return static_cast<T*>(static_cast<void*>(static_cast<char*>(memory_) + off));
    }
private:
    static size_t makeKey(const std::string& key);
private:
    void * memory_;
    size_t size_;
    std::string key_;
    SharedMemoryHolderPrivate * impl_;
    bool owner_;
};

}

#endif // SHAREDMEMORYHOLDER_H
