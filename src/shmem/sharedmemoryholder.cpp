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

#include <cassert>
#include <sstream>
#include <boost/functional/hash.hpp>

#include "sharedmemoryholder.h"
#include "sharedmemoryholderprivate.h"

#ifdef USE_SYSTEMV_SHMEM
#include "systemvsharedmemory.h"
#endif

#ifdef USE_WIN32_SHMEM
#include "win32sharedmemory.h"
#endif


namespace cf {

SharedMemoryHolder::SharedMemoryHolder(bool owner)
    : memory_(NULL),
      size_(0),
      impl_(NULL),
      owner_(owner)
{
#ifdef USE_SYSTEMV_SHMEM
    impl_ = new SystemVSharedMemory;
#endif

#ifdef USE_WIN32_SHMEM
    impl_ = new Win32SharedMemory;
#endif
}

SharedMemoryHolder::~SharedMemoryHolder() {
    try {
        detach();

        if(owner_)
            remove();
    }
    catch(...) {}

    delete impl_;
}

void SharedMemoryHolder::detach()
{
    assert(impl_);

    if(memory_) {
        impl_->close(memory_);
        memory_ = NULL;
    }

    size_ = 0;
}

void SharedMemoryHolder::create(const std::string& key, size_t size)
{
    assert(impl_);

    if(isAttached())
        detach();

    memory_ = impl_->create(makeKey(key), size);

    if(!memory_) {
        size_ = 0;
        std::ostringstream buf;
        buf << "could not create shared memory with key: \"" << key << "\" and size: " << size;

        SharedMemoryHolderPrivate::error_t e = impl_->error();

        if(e == SharedMemoryHolderPrivate::LIMITS)
            throw LowSharedMemoryException(buf.str(), size, impl_->limit());

        // other errors
        throw Exception(buf.str());
    }

    key_ = key;
    size_ = size;
}

void * SharedMemoryHolder::get()
{
    return memory_;
}

size_t SharedMemoryHolder::makeKey(const std::string& key)
{
    boost::hash<std::string> string_hash;
    return string_hash(key);
}

void SharedMemoryHolder::attach(const std::string& key, size_t size)
{
    assert(impl_);

    if(isAttached())
        detach();

    memory_ = impl_->open(makeKey(key), size);

    if(!memory_) {
        std::ostringstream buf;
        buf << "could not attach to memory with key: \"" << key << "\" and size: " << size;
        throw Exception(buf.str());
    }

    key_ = key;
    size_ = size;
}

void SharedMemoryHolder::remove()
{
    assert(impl_);

    if(isAttached())
        detach();

    if(!impl_->remove()) {
        std::ostringstream buf;
        buf << "could not remove shared memory with key: \"" << key_ << "\" and size: " << size_;
        throw Exception(buf.str());
    }
}

size_t SharedMemoryHolder::size() const
{
    return size_;
}

bool SharedMemoryHolder::isAttached() const
{
    return memory_ != NULL;
}

}




