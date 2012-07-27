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

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <signal.h>
#include <string.h>

#include "systemvsharedmemory.h"

namespace cf {

namespace {

struct sigaction old_segv_act;
struct sigaction old_term_act;
int shmem_key = 0;

void unset_crash_handler();

void init_old_action()
{
    memset(&old_segv_act, 0, sizeof(old_segv_act));
    memset(&old_term_act, 0, sizeof(old_term_act));
}

void shmem_segfault_handler(int sig)
{
    switch(sig) {
    case SIGILL:
    case SIGSEGV:
    case SIGTERM: {
        if(shmem_key <= 0) {
            std::cerr << "[SystemVSharedMemory] invalid shmem key:" << shmem_key << "\n";
            std::cerr.flush();
            unset_crash_handler();
            raise(sig);
        }

        errno = 0;
        int rc = shmctl(shmem_key, IPC_RMID, NULL);
        if(rc != 0) {
            std::cerr << "[SystemVSharedMemory] can't remove shmem: "
                      << strerror(errno) << "\n";
        }

        shmem_key = 0;
        unset_crash_handler();
    }
        break;
    default:
        break;
    }
}

struct sigaction make_crash_action()
{
    struct sigaction res;
    memset(&res, 0, sizeof(res));
    res.sa_handler = shmem_segfault_handler;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGILL);
    res.sa_mask = set;
    return res;
}

void set_crash_handler(int key)
{
    struct sigaction act_segv = make_crash_action();
    if(sigaction(SIGSEGV, &act_segv, &old_segv_act) != 0)
        std::cerr << "[SystemVSharedMemory] can't set sigaction for SIGSEGV\n";

    struct sigaction act_term = make_crash_action();
    if(sigaction(SIGTERM, &act_term, &old_term_act) != 0)
        std::cerr << "[SystemVSharedMemory] can't set sigaction for SIGTERM\n";

    struct sigaction act_ill = make_crash_action();
    if(sigaction(SIGILL, &act_ill, NULL) != 0)
        std::cerr << "[SystemVSharedMemory] can't set sigaction for SIGILL\n";

    std::cerr << "shmem id:" << key << "\n";

    shmem_key = key;
}

void unset_crash_handler()
{
    if(sigaction(SIGSEGV, &old_segv_act, NULL) != 0)
        std::cerr << "[SystemVSharedMemory] can't restore sigaction for SIGSEGV\n";
    if(sigaction(SIGTERM, &old_term_act, NULL) != 0)
        std::cerr << "[SystemVSharedMemory] can't restore sigaction for SIGTERM\n";


    struct sigaction act_ill;
    memset(&act_ill, 0, sizeof(act_ill));
    act_ill.sa_handler = SIG_DFL;

    if(sigaction(SIGILL, &act_ill, NULL) != 0)
        std::cerr << "[SystemVSharedMemory] can't restore sigaction for SIGILL\n";

    shmem_key = 0;
}

}

SystemVSharedMemory::SystemVSharedMemory() : id_(0)
{
}

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

    set_crash_handler(id_);

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
    unset_crash_handler();
    int rc = shmctl(id_, IPC_RMID, NULL);
    return rc == 0;
}

size_t SystemVSharedMemory::limit() const
{
    size_t shmmax = 0;
    size_t len = sizeof(shmmax);

#ifdef __APPLE__
    const char * KEY = "kern.sysv.shmmax";
    int result = sysctlbyname(KEY, &shmmax, &len, NULL, 0);
    return result == -1 ? 0 : shmmax;
#elif __NetBSD__
    int names[4];
    names[0] = CTL_KERN;
    names[1] = KERN_SYSVIPC;
    names[2] = KERN_SYSVIPC_INFO;    
    names[3] = KERN_SYSVIPC_SHM_INFO;
   
    len = sizeof(shminfo);

    void * buf = calloc(len, 1);

    shm_sysctl_info * shmsi = (shm_sysctl_info*) buf;
    int result = sysctl(names, 4, shmsi, &len, NULL, 0);
    if(result == -1)
        perror("sysctl error: ");

    shmmax = shmsi->shminfo.shmmax;
    free(buf);

    return result == -1 ? 0 : shmmax;
#elif __OpenBSD__
    int names[3];
    int names_sz = 3;
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
