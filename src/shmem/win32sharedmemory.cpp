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

#include <cstdio>
#include <sstream>
#include <Windows.h>

#include "win32sharedmemory.h"

namespace cf {

std::string makeKey(size_t id)
{
    std::ostringstream os;
    os << "key" << id;
    return os.str();
}

Win32SharedMemory::Win32SharedMemory()
        : handle_(NULL)
{
}

void Win32SharedMemory::close(void * mem)
{
    if(!UnmapViewOfFile(mem))
        fprintf(stderr, TEXT("Could not unmap memory (%d).\n"), GetLastError());
}

void * Win32SharedMemory::create(size_t key, size_t size)
{
    handle_ = CreateFileMapping(INVALID_HANDLE_VALUE,    // use paging file
                                NULL,                    // default security
                                PAGE_READWRITE,          // read/write access
                                0,                       // maximum object size (high-order DWORD)
                                size,                    // maximum object size (low-order DWORD)
                                makeKey(key).c_str());   // name of mapping object

    if(handle_ == NULL) {
        fprintf(stderr, TEXT("Could not create file mapping object (%d).\n"), GetLastError());
        return NULL;
    }

    void * mem = MapViewOfFile(handle_,   // handle to map object
                               FILE_MAP_ALL_ACCESS, // read/write permission
                               0,
                               0,
                               size);

    if(mem == NULL) {
        fprintf(stderr, TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(handle_);
        return NULL;
    }

    return mem;
}

void * Win32SharedMemory::open(size_t key, size_t size)
{
    handle_ = OpenFileMapping(FILE_MAP_ALL_ACCESS,   // read/write access
                              FALSE,                 // do not inherit the name
                              makeKey(key).c_str()); // name of mapping object

    if(handle_ == NULL) {
        fprintf(stderr, TEXT("Could not open file mapping object (%d).\n"), GetLastError());
        return NULL;
    }

    void * mem = MapViewOfFile(handle_, // handle to map object
                               FILE_MAP_ALL_ACCESS,  // read/write permission
                               0,
                               0,
                               size);

    if(mem == NULL) {
        fprintf(stderr, "Could not map view of file (%d).\n", GetLastError());
        CloseHandle(handle_);
        return NULL;
    }

    return mem;
}

bool Win32SharedMemory::remove()
{
    BOOL ret = CloseHandle(handle_);

    handle_ = NULL;

    if(!ret) {
        fprintf(stderr, TEXT("Could not close shared memory handle (%d).\n"), GetLastError());
        return false;
    }

    return true;
}

}
