set(CF_HEADER_INSTALL "cuneiform")
set(RELATIVE_DATADIR "share/cuneiform")
set(INSTALL_DATADIR "${CMAKE_INSTALL_PREFIX}/${RELATIVE_DATADIR}")
set(CF_CAN_HIDE_VISIBILITY OFF)

if(NOT MSVC AND NOT MINGW)
    check_cxx_compiler_flag(-fvisibility=hidden HAVE_GCC_VISIBILITY)
    if(HAVE_GCC_VISIBILITY)
        set(CF_CAN_HIDE_VISIBILITY ON)
    endif()
endif()

include(CheckTypeSize)

check_type_size("void*" PTRSIZE)
if(PTRSIZE EQUAL 8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBDIR "lib64")
    if(UNIX AND NOT WIN32)
        add_definitions(-fPIC)
    endif()
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

function(cf_install_library name)
    if(NOT WIN32)
        set_target_properties(${name} PROPERTIES VERSION ${CF_VERSION})
    endif()
  
    install(
        TARGETS "${name}"
        COMPONENT Libraries
        ARCHIVE DESTINATION ${LIBDIR}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIBDIR}
    )
endfunction()

function(cf_install_exe name)
    install(
        TARGETS "${name}"
        RUNTIME DESTINATION bin
        COMPONENT Applications
    )
endfunction()

# usage: cf_install_headers(DestinationDir Files)
function(cf_install_headers)
    set(dir "include/${CF_HEADER_INSTALL}/${ARGV0}")
    set(files ${ARGN})
    if(NOT "${ARGV0}" STREQUAL "")
        list(REMOVE_AT files 0)
    endif()
    foreach(f ${files})
        install(FILES ${f} 
            DESTINATION "${dir}"
            COMPONENT Headers) 
    endforeach()
endfunction()

function(cf_hide_visibility name)
    if(CF_CAN_HIDE_VISIBILITY)
        set_property(TARGET ${name} PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
    endif()
endfunction()
