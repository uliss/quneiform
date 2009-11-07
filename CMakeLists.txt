project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

SET(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -fno-inline -Wall -Wextra -fvisibility=default")

add_definitions("-D_USE_RVERLINE_")

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

set(CF_VERSION_MAJOR "0")
set(CF_VERSION_MINOR "8")
set(CF_VERSION_PATCH "0")

set(CF_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")

include(install_files.cmake)

# Stop MSVC complaints about POSIX functions.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT MSVC60)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    endif()
endif()

find_package(ImageMagick COMPONENTS Magick++)
if(ImageMagick_Magick++_FOUND)
    include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
    message(STATUS "ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
    set(cli_ext_libs ${ImageMagick_Magick++_LIBRARY})
else()
    message(FATAL_ERROR "ImageMagick++ required")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/include/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/include)

# Libdl is needed only on Apple and Linux (that I know of).
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    find_library(DL_LIB dl)
endif()

# Emulate Windows DLL symbol resolve order.
if(UNIX AND NOT APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()

if(MINGW)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mwindows")
endif()

add_subdirectory(datafiles)
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(tests)

