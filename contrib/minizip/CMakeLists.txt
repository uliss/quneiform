include(FindZLIB)

if(NOT ZLIB_FOUND)
    message(ERROR "ZLib not found.")
endif()

set(SOURCES
    ioapi.c
    zip.c)

include_directories(${ZLIB_INCLUDE_DIRS})
add_definitions(-DNOCRYPT -DUSE_FILE32API)
add_library(minizip STATIC ${SOURCES})
target_link_libraries(minizip ${ZLIB_LIBRARIES})
