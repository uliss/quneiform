#ifndef SHARED_MEMORY_TYPE_H
#define SHARED_MEMORY_TYPE_H

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif
#include <boost/interprocess/managed_shared_memory.hpp>

#ifdef _WIN32
#include <boost/interprocess/managed_windows_shared_memory.hpp>
#endif

namespace cf {
#ifdef _WIN32
    typedef boost::interprocess::managed_windows_shared_memory SharedMemory;
#else
    typedef boost::interprocess::managed_shared_memory SharedMemory;
#endif
}

#endif // SHARED_MEMORY_TYPE_H
