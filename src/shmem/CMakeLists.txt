set(SHMEM_SOURCES
    memorydata.cpp
    sharedmemoryholder.cpp
)

set(SHMEM_HEADERS
    sharedmemoryholderprivate.h
)

set(CF_SHMEM_LOAD_LIBS
    cuneiform-common
    cuneiform-export
    cuneiform-load
    ${Boost_SERIALIZATION_LIBRARY}
)

if(UNIX)
    add_definitions(-DUSE_SYSTEMV_SHMEM)
    list(APPEND SHMEM_SOURCES systemvsharedmemory.cpp)
endif()

if(WIN32)
    add_definitions(-DUSE_WIN32_SHMEM)
    list(APPEND SHMEM_SOURCES win32sharedmemory.cpp)
endif()

add_library(cuneiform-shmem STATIC 
    ${SHMEM_SOURCES}
    ${SHMEM_HEADERS}
)

target_link_libraries(cuneiform-shmem ${CF_SHMEM_LOAD_LIBS})
