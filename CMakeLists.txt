project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
 
include(outtree)
include(build_number)
include(version)
include(uninstall)
include(update_samples)
include(options)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
if(WIN32)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif()

include(CheckCXXCompilerFlag)

include(msvc)
include(flags_common)
include(flags_release)
include(flags_debug)
include(install_files)
include(library_link)
include(find_image_libs)
    
# Iconv
find_package(Iconv)
if(NOT ICONV_FOUND)
    message(FATAL_ERROR "Iconv library required")
endif()

# Boost
if(WIN32)
    set(Boost_USE_STATIC_LIBS On)
endif()

find_package(Boost 1.33)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    find_package(Boost 1.33 COMPONENTS serialization)

    if(Boost_SERIALIZATION_FOUND)
        set(CF_SERIALIZE True)
    endif()
else()
    message(FATAL_ERROR "Boost library required")
endif()

include(use_doxygen)
include(make_config)
include(make_package)

add_subdirectory(datafiles)
add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(cli)

if(CF_NO_GUI)
    message(STATUS "No GUI will be build")
    set(CF_GUI False)
else()
    add_subdirectory(gui)
    message(STATUS "Qt GUI will be build")
    set(CF_GUI True)
endif()

add_subdirectory(utils)

if(TESTS)
   ENABLE_TESTING()
   add_subdirectory(tests)
endif()

