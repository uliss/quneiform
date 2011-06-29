include(FindZLIB)

if(NOT ZLIB_FOUND)
    message(ERROR "ZLib not found.")
endif()

set(SOURCES
    ioapi.c
    zip.c)

include_directories(${ZLIB_INCLUDE_DIRS})
add_definitions(-DNOCRYPT)
add_library(minizip STATIC ${SOURCES})
