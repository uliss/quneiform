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

#include "systemvsharedmemory.h"

namespace cf {

SystemVSharedMemory::SystemVSharedMemory() : id_(0) {}

void SystemVSharedMemory::close(void * mem)
{
    shmdt(mem);
}

void * SystemVSharedMemory::create(int k, size_t size)
{
    key_t key = k;
    id_ = shmget(key, size, IPC_CREAT | 0666);

    if (id_ < 0)
        return NULL;

    void * memory = shmat(id_, NULL, 0);

    if (memory == (char *) -1)
        return NULL;

    return memory;
}

void * SystemVSharedMemory::open(int k)
{
    key_t key = k;
    id_ = shmget(key, 0, 0666);

    if(id_ < 0)
        return NULL;

    void * memory = shmat(id_, NULL, 0);

    if (memory == (char *) -1)
        return NULL;

    return memory;
}

bool SystemVSharedMemory::remove()
{
    return shmctl(id_, IPC_RMID, NULL) != -1;
}

}
