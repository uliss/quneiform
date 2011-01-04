set(CF_HEADER_INSTALL "cuneiform")
set(RELATIVE_DATADIR "share/cuneiform")
set(INSTALL_DATADIR "${CMAKE_INSTALL_PREFIX}/${RELATIVE_DATADIR}")

include(CheckTypeSize)
check_type_size("void*" PTRSIZE)
if(PTRSIZE EQUAL 8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBDIR "lib64")
else()
    set(LIBDIR "lib")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    if(EXISTS /usr/include/dlfcn.h)
        SET(CMAKE_DL_LIBS "")
        SET(CMAKE_SHARED_LIBRARY_C_FLAGS "-fPIC")            # -pic 
        SET(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-shared")       # -shared
        SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")         # +s, flag for exe link to use shared lib
        SET(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "-Wl,-rpath,")       # -rpath
        SET(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP ":")   # : or empty
        SET(CMAKE_SHARED_LIBRARY_RPATH_LINK_C_FLAG "-Wl,-rpath-link,")
        SET(CMAKE_SHARED_LIBRARY_SONAME_C_FLAG "-Wl,-soname,")
        SET(CMAKE_SHARED_LIBRARY_SONAME_CXX_FLAG "-Wl,-soname,")
        SET(CMAKE_EXE_EXPORTS_C_FLAG "-Wl,--export-dynamic")
        SET(CMAKE_EXE_EXPORTS_CXX_FLAG "-Wl,--export-dynamic")
     endif()
endif()

macro(library_hook name)
  if(NOT WIN32)
    set_target_properties(${name} PROPERTIES VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}")
  endif()
  
  install(
    TARGETS "${name}"
    ARCHIVE DESTINATION ${LIBDIR}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBDIR}
  )
endmacro()

macro(install_exe name)
    install(
        TARGETS "${name}"
        RUNTIME DESTINATION bin
        COMPONENT applications
    )
endmacro()

macro(cif_visibility_hidden name)
  CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_GCC_VISIBILITY)
  if(HAVE_GCC_VISIBILITY)
      set_property(TARGET ${name} PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
  endif() 
endmacro()
