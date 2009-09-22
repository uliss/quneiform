include_directories(
	Kern
	Kern/include
Kern/h
Kern/hh
Kern/hhh
Kern/hdebug
Kern/hrk
Kern/hrkint
Kern/hrstr
Kern/fon/src
Kern/usage
)

# Hide non exported functions/variables
include(CheckCCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    #check_c_compiler_flag("-fvisibility=hidden -DHAVE_GCCVISIBILITY" HAVE_GCCVISIBILITY)
    #    if (HAVE_GCCVISIBILITY)
    #	    add_definitions("-fvisibility=hidden -DHAVE_GCCVISIBILITY")
    #endif()
endif()

# Stop MSVC complaints about POSIX functions.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT MSVC60)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    endif()
endif()

add_definitions(-D_USE_RVERLINE_ -Wall -Wextra)

find_package(ImageMagick COMPONENTS Magick++)
if(ImageMagick_Magick++_FOUND)
    set(USE_MAGICK TRUE)
    include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
    message(STATUS "ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
    set(cli_ext_libs ${ImageMagick_Magick++_LIBRARY})
else()
    set(USE_MAGICK FALSE)
    message(STATUS "ImageMagick++ not found. Only uncompressed BMP images supported.")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Kern/include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/Kern/include/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Kern/include)

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

add_subdirectory(Kern)
add_subdirectory(cli)
