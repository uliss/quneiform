/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <cstddef>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <boost/current_function.hpp>

#include "mmapsharedmemory.h"
#include "common/filesystem.h"
#include "common/log.h"
#include "common/helper.h"

namespace cf {

#define SHMEM_ERROR_FUNC() cfError(cf::MODULE_SHMEM) << METHOD_SIGNATURE()
#define SHMEM_TRACE_FUNC() cfTrace(cf::MODULE_SHMEM) << METHOD_SIGNATURE()

static std::string makePath(size_t key)
{
    std::ostringstream buf;
    buf << "/tmp/cf_mmap_" << key;
    return buf.str();
}

static bool createFile(const std::string& fullPath, size_t sz)
{
    const long page_size = ::sysconf(_SC_PAGESIZE);
    const size_t buf_size = ((sz / page_size) + 1) * page_size;
    void * buf = calloc(1, buf_size);

    int fd = open(fullPath.c_str(), O_CREAT | O_RDWR, 0660);
    if (fd == -1) {
        SHMEM_ERROR_FUNC() << "can't create file:" << fullPath;
        free(buf);
        return false;
    }

    int rc = write(fd, buf, buf_size);

    if(rc != buf_size) {
        SHMEM_ERROR_FUNC() << "can't write to file" << fullPath
                              << ", " << buf_size << "bytes";
        ::close(fd);
        free(buf);
        return false;
    }

    if(::close(fd) != 0)
        SHMEM_ERROR_FUNC() << "can't close file:" << fullPath;

    free(buf);
    return true;
}

MMapSharedMemory::MMapSharedMemory() :
    mmap_mem_(NULL),
    mmap_size_(0),
    error_(OK)
{
}

void MMapSharedMemory::close(void * mem)
{
    if(mmap_mem_)
        munmap(mmap_mem_, mmap_size_);
}

void * MMapSharedMemory::create(size_t key, size_t size)
{
    SHMEM_TRACE_FUNC() << "create mmap with key:" << key << "and size:" << size;

    std::string fpath = makePath(key);
    if(!fs::fileExists(fpath)) {
        bool rc = createFile(fpath, size);
        if(!rc) {
            SHMEM_ERROR_FUNC() << "file creation failed:" << fpath;
            return NULL;
        }
    }

    return open(key, size);
}

void * MMapSharedMemory::open(size_t key, size_t size)
{
    SHMEM_TRACE_FUNC() << "open mmap with key:" << key << "and size:" << size;

    std::string path = makePath(key);

    int fd = ::open(path.c_str(), O_RDWR);
    if(fd < 1) {
        SHMEM_ERROR_FUNC() << "can't open" << path << ":" << strerror(errno);
        return NULL;
    }

    mmap_size_ = size;
    mmap_mem_ = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
    if(mmap_mem_ == MAP_FAILED) {
        SHMEM_ERROR_FUNC() << "can't' mmap:" << path << ":" << strerror(errno);
        return NULL;
    }

    // once the file is mapped, we can dispose of the filehandle
    if(::close(fd) != 0)
        SHMEM_ERROR_FUNC() << "can't close file descriptor" << fd;

    return mmap_mem_;
}

bool MMapSharedMemory::remove(size_t key)
{
    close(mmap_mem_);

    std::string path = makePath(key);
    int rc = ::unlink(path.c_str());
    if(rc == 0)
        return true;

    SHMEM_ERROR_FUNC() << "can't remove" << path;
    return false;
}

size_t MMapSharedMemory::limit() const
{
    return 100000000000;
}

SharedMemoryHolderPrivate::error_t MMapSharedMemory::error() const
{
    return error_;
}

}
