# /kde/kdesupport/strigi/cmake
# - Try to find Iconv 
# Once done this will define 
# 
#  ICONV_FOUND - system has Iconv 
#  ICONV_INCLUDE_DIR - the Iconv include directory 
#  ICONV_LIBRARIES - Link these to use Iconv 
#  ICONV_SECOND_ARGUMENT_IS_CONST - the second argument for iconv() is const
# 
include(CheckCXXSourceCompiles)

IF (ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)
  # Already in cache, be silent
  SET(ICONV_FIND_QUIETLY TRUE)
ENDIF (ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)

if(APPLE) 
    FIND_PATH(ICONV_INCLUDE_DIR NAMES iconv.h PATHS /opt/local/include NO_DEFAULT_PATH) 
    FIND_LIBRARY(ICONV_LIBRARIES NAMES iconv libiconv libiconv-2 PATHS /opt/local/lib NO_DEFAULT_PATH)
else()
    FIND_PATH(ICONV_INCLUDE_DIR iconv.h
		PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\LibIconv;InstallPath]/include"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GnuWin32\\LibIconv;InstallPath]/include"
	)
    FIND_LIBRARY(ICONV_LIBRARIES 
		NAMES iconv libiconv libiconv-2 c
		PATHS
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\LibIconv;InstallPath]/lib"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GnuWin32\\LibIconv;InstallPath]/lib"
	)
endif()

message(STATUS "iconv include path found: ${ICONV_INCLUDE_DIR}")
message(STATUS "iconv library found: ${ICONV_LIBRARIES}")
 
IF(ICONV_INCLUDE_DIR AND ICONV_LIBRARIES) 
  SET(ICONV_FOUND TRUE) 
ENDIF(ICONV_INCLUDE_DIR AND ICONV_LIBRARIES) 

set(CMAKE_REQUIRED_INCLUDES ${ICONV_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES ${ICONV_LIBRARIES})
IF(ICONV_FOUND)
  if(MSVC)
	set(ICONV_SECOND_ARGUMENT_IS_CONST 1)
  else()
	check_cxx_source_compiles("
    #include <iconv.h>
    int main(){
		iconv_t conv = 0;
		const char* in = 0;
		size_t ilen = 0;
		char* out = 0;
		size_t olen = 0;
		iconv(conv, &in, &ilen, &out, &olen);
		return 0;
    }
    " ICONV_SECOND_ARGUMENT_IS_CONST )
  endif()
ENDIF(ICONV_FOUND)
set(CMAKE_REQUIRED_INCLUDES)
set(CMAKE_REQUIRED_LIBRARIES)

IF(ICONV_FOUND) 
  IF(NOT ICONV_FIND_QUIETLY) 
    MESSAGE(STATUS "Found Iconv: ${ICONV_LIBRARIES}") 
  ENDIF(NOT ICONV_FIND_QUIETLY) 
ELSE(ICONV_FOUND) 
  IF(Iconv_FIND_REQUIRED) 
    MESSAGE(FATAL_ERROR "Could not find Iconv") 
  ENDIF(Iconv_FIND_REQUIRED) 
ENDIF(ICONV_FOUND) 
