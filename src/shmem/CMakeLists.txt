set(SHMEM_SOURCES
    memorydata.cpp
    sharedmemoryholder.cpp
    systemvsharedmemory.cpp
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
    list(APPEND SHMEM_RESOURCES systemvsharedmemory.cpp)
endif()

add_library(cuneiform-shmem ${LIBTYPE}
    ${SHMEM_SOURCES}
    ${SHMEM_HEADERS}
)

target_link_libraries(cuneiform-shmem ${CF_SHMEM_LOAD_LIBS})
