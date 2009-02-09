project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir and try again.")
endif()

set(CF_VERSION_MAJOR "0")
set(CF_VERSION_MINOR "6")
set(CF_VERSION_PATCH "0")

set(CF_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")

if(BUILD_TESTING)
  enable_testing()
  include(CTest)
  include(Dart)
endif(BUILD_TESTING)

include(install_files.cmake)

add_subdirectory(datafiles)
add_subdirectory(cuneiform_src)
