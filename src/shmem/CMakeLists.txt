add_definitions(-DBOOST_DATE_TIME_NO_LIB)

set(CF_SHMEM_SOURCE
    memorydata.cpp
    sharedmemoryholder.cpp
)

set(CF_SHMEM_HEADERS
    sharedmemoryholderprivate.h
)

set(CF_SHMEM_LINK
    ${CF_COMMON}
    ${CF_EXPORT}
    ${CF_LOAD}
    ${Boost_SERIALIZATION_LIBRARY}
)

if(USE_MMAP_SHMEM)
    add_definitions(-DUSE_MMAP_SHMEM)
    list(APPEND CF_SHMEM_SOURCE mmapsharedmemory.cpp)
elseif(USE_SYSTEMV_SHMEM)
    add_definitions(-DUSE_SYSTEMV_SHMEM)
    list(APPEND CF_SHMEM_SOURCE systemvsharedmemory.cpp)
    if(CF_LIB_RT_HAS_SHMOPEN)
        list(APPEND CF_SHMEM_LINK rt)
    endif()
elseif(USE_WIN32_SHMEM)
    add_definitions(-DUSE_WIN32_SHMEM)
    list(APPEND CF_SHMEM_SOURCE win32sharedmemory.cpp)
endif()

add_library(${CF_SHMEM} STATIC
    ${CF_SHMEM_SOURCE}
    ${CF_SHMEM_HEADERS}
)

target_link_libraries(${CF_SHMEM} ${CF_SHMEM_LINK})
cf_cppcheck(${CF_SHMEM})
