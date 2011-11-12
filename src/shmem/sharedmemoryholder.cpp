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

SharedMemoryHolder::SharedMemoryHolder()
    : memory_(NULL),
      size_(0),
      impl_(NULL)
{
#ifdef USE_SYSTEMV_SHMEM
    impl_ = new SystemVSharedMemory;
#endif

#ifdef USE_WIN32_SHMEM
    impl_ = new Win32SharedMemory;
#endif
}

SharedMemoryHolder::~SharedMemoryHolder() {
    detach();
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

    key_ = key;
    memory_ = impl_->create(makeKey(), size);

    if(!memory_) {
        size_ = 0;
        std::ostringstream buf;
        buf << "Can't create shared memory with key: \"" << key << "\" and size: " << size << std::endl;
        throw Exception(buf.str());
    }

    size_ = size;
}

void * SharedMemoryHolder::get()
{
    return memory_;
}

int SharedMemoryHolder::makeKey() const
{
    boost::hash<std::string> string_hash;
    return string_hash(key_);
}

void SharedMemoryHolder::attach(const std::string& key, size_t size)
{
    assert(impl_);

    key_ = key;
    memory_ = impl_->open(makeKey());
    size_ = size;
}

void SharedMemoryHolder::remove()
{
    assert(impl_);

    if(isAttached())
        detach();

    if(!impl_->remove()) {
        std::ostringstream buf;
        buf << "Can't remove shared memory with key: \"" << key_ << "\" and size: " << size_ << std::endl;
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




