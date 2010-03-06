#
# Find the libzip includes and library
#
# This module defines
# ZIP_INCLUDE_DIR, where to find zip.h, etc.
# ZIP_LIBRARIES, the libraries to link against to use libzip.
# ZIP_FOUND, If false, do not try to use libzip.

find_path(ZIP_INCLUDE_DIR zip.h
  /usr/local/include
  /usr/include
)

find_library(ZIP_LIBRARY zip ${ZIP_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib)

if(ZIP_INCLUDE_DIR)
   if(ZIP_LIBRARY)
      set(ZIP_FOUND "YES")
      set(ZIP_LIBRARIES ${ZIP_LIBRARY} ${CMAKE_DL_LIBS})
      message(STATUS "libzip found at ${ZIP_INCLUDE_DIR}") 
   endif()
else() 
   message(STATUS "libzip include directory not found")
endif()

