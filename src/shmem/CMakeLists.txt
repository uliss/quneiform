add_definitions(-DBOOST_DATE_TIME_NO_LIB)

set(SHMEM_SOURCES
    memorydata.cpp
    sharedmemoryholder.cpp
)

set(SHMEM_HEADERS
    sharedmemoryholderprivate.h
)

set(CF_SHMEM_LIBS
    cuneiform-common
    cuneiform-export
    cuneiform-load
    ${Boost_SERIALIZATION_LIBRARY}
)

if(USE_SYSTEMV_SHMEM)
    add_definitions(-DUSE_SYSTEMV_SHMEM)
    list(APPEND SHMEM_SOURCES systemvsharedmemory.cpp)
    if(CF_LIB_RT_HAS_SHMOPEN)
       list(APPEND CF_SHMEM_LIBS rt)
    endif()
endif()

if(WIN32)
    add_definitions(-DUSE_WIN32_SHMEM)
    list(APPEND SHMEM_SOURCES win32sharedmemory.cpp)
endif()

add_library(cuneiform-shmem STATIC 
    ${SHMEM_SOURCES}
    ${SHMEM_HEADERS}
)

target_link_libraries(cuneiform-shmem ${CF_SHMEM_LIBS})
