#
# Find the CppUnit includes and library
#
# This module defines
# CPPUNIT_INCLUDE_DIR, where to find tiff.h, etc.
# CPPUNIT_LIBRARIES, the libraries to link against to use CppUnit.
# CPPUNIT_FOUND, If false, do not try to use CppUnit.

# also defined, but not for general use are
# CPPUNIT_LIBRARY, where to find the CppUnit library.
# CPPUNIT_DEBUG_LIBRARY, where to find the CppUnit library in debug mode.

find_path(CPPUNIT_INCLUDE_DIR "cppunit/TestCase.h"
  /usr/local/include
  /usr/include
)

# With Win32, important to have both
IF(WIN32)
   FIND_LIBRARY(CPPUNIT_LIBRARY cppunit ${CPPUNIT_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib)
   FIND_LIBRARY(CPPUNIT_DEBUG_LIBRARY cppunitd ${CPPUNIT_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib)
ELSE(WIN32)
# On unix system, debug and release have the same name
   FIND_LIBRARY(CPPUNIT_LIBRARY cppunit ${CPPUNIT_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib)
   FIND_LIBRARY(CPPUNIT_DEBUG_LIBRARY cppunit ${CPPUNIT_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib)
ENDIF(WIN32)

if(CPPUNIT_INCLUDE_DIR)
   if(CPPUNIT_LIBRARY)
      SET(CPPUNIT_FOUND "YES")
      SET(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} ${CMAKE_DL_LIBS})
      SET(CPPUNIT_DEBUG_LIBRARIES ${CPPUNIT_DEBUG_LIBRARY} ${CMAKE_DL_LIBS})
   else()
      message(STATUS "CppUnit: library not found.")
   endif()
else() 
	message(STATUS "CppUnit: include directory not found.")
endif()
