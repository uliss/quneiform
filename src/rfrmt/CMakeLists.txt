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

add_library(cuneiform-rfrmt ${LIBTYPE}
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

cif_visibility_hidden(cuneiform-rfrmt)
  
set_property(TARGET cuneiform-rfrmt PROPERTY COMPILE_DEFINITIONS __RFRMT__)

target_link_libraries(cuneiform-rfrmt
    cuneiform-compat
    cuneiform-common
    cuneiform-ced
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rdib
    cuneiform-rimage
    cuneiform-cline
    cuneiform-cstr
)

cf_install_library(cuneiform-rfrmt)
install_headers(rfrmt ${CF_RFRMT_HEADERS_INSTALL})

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-rfrmt UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
