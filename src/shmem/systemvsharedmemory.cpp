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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#include "systemvsharedmemory.h"

namespace cf {

SystemVSharedMemory::SystemVSharedMemory() : id_(0) {}

void SystemVSharedMemory::close(void * mem)
{
    shmdt(mem);
}

void * SystemVSharedMemory::create(size_t key, size_t size)
{
    id_ = shmget((key_t) key, size, IPC_CREAT | 0666);

    if (id_ < 0) {
        switch(errno) {
        case EINVAL:
            error_ = LIMITS;
            break;
        case EACCES:
            error_ = NO_ACCESS;
            break;
        case ENOMEM:
            error_ = NO_MEMORY;
            break;
        default:
            error_ = LIMITS;
        }

        return NULL;
    }

    void * memory = shmat(id_, NULL, 0);

    if (memory == (char *) -1) {
        return NULL;
    }

    return memory;
}

void * SystemVSharedMemory::open(size_t key, size_t)
{
    id_ = shmget((key_t) key, 0, 0666);

    if(id_ < 0)
        return NULL;

    void * memory = shmat(id_, NULL, 0);

    if (memory == (char *) -1)
        return NULL;

    return memory;
}

bool SystemVSharedMemory::remove()
{
    return shmctl(id_, IPC_RMID, NULL) == 0;
}

size_t SystemVSharedMemory::limit() const
{
    size_t shmmax = 0;
    size_t len = sizeof(shmmax);

#ifdef __APPLE__
    const char * KEY = "kern.sysv.shmmax";
    int result = sysctlbyname(KEY, &shmmax, &len, NULL, 0);
    return result == -1 ? 0 : shmmax;
#elif __OpenBSD__
    int names[3];
    int names_sz = sizeof(names);
    names[0] = CTL_KERN;
    names[1] = KERN_SHMINFO;
    names[2] = KERN_SHMINFO_SHMMAX;    

    int result = sysctl(names, names_sz, &shmmax, &len, NULL, 0);
    return result == -1 ? 0 : shmmax;
#elif __linux__
    int names[2];
    int names_sz = sizeof(names);
    names[0] = CTL_KERN;
    names[1] = KERN_SHMMAX;
    
    int result = sysctl(names, names_sz, &shmmax, &len, NULL, 0);
    return result == -1 ? 0 : shmmax;
#else
    return 0;
#endif
}

SharedMemoryHolderPrivate::error_t SystemVSharedMemory::error() const
{
    return error_;
}

}
