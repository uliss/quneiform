project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
if(WIN32)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif()

include(CheckCXXCompilerFlag)
macro(SET_COMPILER_DEBUG_FLAG flag)
    CHECK_CXX_COMPILER_FLAG(${flag} "HAVE_CXX_COMPILER_FLAG${flag}")
    if("HAVE_CXX_COMPILER_FLAG${flag}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}")
    else()
        message(STATUS "Compiler flag ${flag} is not supported")
    endif()
endmacro()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    SET_COMPILER_DEBUG_FLAG(/Wall)
    SET_COMPILER_DEBUG_FLAG(-O0)
    SET_COMPILER_DEBUG_FLAG(-fvisibility=default)
    SET_COMPILER_DEBUG_FLAG(-Wall)
    SET_COMPILER_DEBUG_FLAG(-Wextra)
    SET_COMPILER_DEBUG_FLAG(-Wconversion)
    SET_COMPILER_DEBUG_FLAG(-fno-inline)

    # setting linker warning flags
    if(CMAKE_SYSTEM_NAME STREQUAL LINUX)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--warn-common")
    endif()

    message(STATUS "Compiler flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

add_definitions("-D_USE_RVERLINE_")

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

set(CF_VERSION_MAJOR "0")
set(CF_VERSION_MINOR "8")
set(CF_VERSION_PATCH "9999")
set(CF_HEADER_INSTALL "cuneiform")

set(CF_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")

include(install_files.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

# Stop MSVC complaints about POSIX functions.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT MSVC60)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    endif()
endif()

if(NOT NO_IMAGE_MAGICK)
find_package(ImageMagick COMPONENTS Magick++)
if(ImageMagick_Magick++_FOUND)
    include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
    message(STATUS "ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
    message(STATUS "ImageMagick++ library is ${ImageMagick_Magick++_LIBRARY}.")    
    set(USE_IMAGE_MAGICK TRUE)
endif()
endif()

find_package(Iconv)
if(ICONV_FOUND)
    set(USE_ICONV TRUE)
endif()

find_package(Boost 1.33)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost library required")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/include/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/include)

# Emulate Windows DLL symbol resolve order.
if(UNIX AND NOT APPLE)
 # set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()

if(MINGW)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mwindows")
endif()

add_subdirectory(datafiles)
add_subdirectory(src)
add_subdirectory(cli)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
   ENABLE_TESTING()
   add_subdirectory(tests)
endif()

set(CPACK_SYSTEM_NAME i686)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cuneiform optical recognition software")
set(CPACK_PACKAGE_VENDOR "Serj Poltavski")
set(CPACK_PACKAGE_VERSION_MAJOR ${CF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CF_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CF_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_CONTACT "serj.poltavski@gmail.com")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  #set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  #set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Cuneiform Optical Recognition Software")
  #set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\code.launchpad.net/cuneiform-linux")
  #set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\code.launchpad.net/cuneiform-linux")
  #set(CPACK_NSIS_CONTACT "serj.poltavski@gmail.com")
  #set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_BINARY_DEB ON)
  set(CPACK_BINARY_RPM OFF)
  set(CPACK_BINARY_TGZ ON)
  set(CPACK_BINARY_TBZ2 ON)
  set(CPACK_BINARY_TZ OFF)
  set(CPACK_BINARY_STGZ OFF)
  set(CPACK_STRIP_FILES "bin/cuneiform")
endif()
include(CPack)


