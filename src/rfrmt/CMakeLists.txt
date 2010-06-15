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
formatter.cpp
frmtfile.cpp
frmtpict.cpp
formatoptions.cpp
pagetree.cpp
rfrmtoptions.cpp
sort_int.cpp
util_lst.cpp
util_spl.cpp
)

cif_visibility_hidden(cuneiform-rfrmt)
  
set_property(TARGET cuneiform-rfrmt PROPERTY COMPILE_DEFINITIONS __RFRMT__)

set(RFRMT_HEADER_INSTALL "rfrmt")
install(FILES 
    crtfchar.h
    crtfword.h
    crtfstring.h
    crtfverticalcolumn.h
    crtfhorizontalcolumn.h
    crtfsector.h
    crtfpage.h
    crtffragment.h
    crtfstruct.h
    formatoptions.h 
    rfrmtoptions.h
    DESTINATION 
    include/${CF_HEADER_INSTALL}/${RFRMT_HEADER_INSTALL})

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

library_hook(cuneiform-rfrmt)
