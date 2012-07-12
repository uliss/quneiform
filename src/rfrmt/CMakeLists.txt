set(CF_RFRMT_HEADERS_INSTALL
    crtfchar.h
    crtfword.h
    crtfstring.h
    crtfverticalcolumn.h
    crtfhorizontalcolumn.h
    crtfsector.h
    crtfpage.h
    crtffragment.h
    crtfstruct.h
    rfrmtoptions.h
)

add_library(${CF_RFRMT} ${LIBTYPE}
    crtfchar.cpp
    crtffragment.cpp
    crtffunc.cpp
    crtfpage.cpp
    crtfsector.cpp
    crtfstring.cpp
    crtfhorizontalcolumn.cpp
    crtfverticalcolumn.cpp
    crtfword.cpp
    formatdebug.cpp
    formatter.cpp
    frmtfile.cpp
    frmtpict.cpp
    pagetree.cpp
    rfrmtoptions.cpp
    sort_int.cpp
    util_lst.cpp
    util_spl.cpp
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    cf_hide_visibility(${CF_RFRMT})
endif()

set_property(TARGET ${CF_RFRMT} PROPERTY COMPILE_DEFINITIONS __RFRMT__)

target_link_libraries(${CF_RFRMT}
    ${CF_COMPAT}
    ${CF_COMMON}
    ${CF_CED}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_RDIB}
    ${CF_RIMAGE}
    ${CF_CLINE}
    ${CF_CSTR}
)

cf_install_library(${CF_RFRMT})
cf_install_headers(rfrmt ${CF_RFRMT_HEADERS_INSTALL})
cf_cppcheck(${CF_RFRMT})
