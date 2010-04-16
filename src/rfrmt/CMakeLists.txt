add_library(cuneiform-rfrmt ${LIBTYPE}
${SNP_SRC}
creatertf.cpp
frmt.cpp
frmtallignment.cpp
frmtdibapi.cpp
frmtdll.cpp
frmtfile.cpp
frmtpict.cpp
formatoptions.cpp
int_file.cpp
mymem.cpp
pagetree.cpp
rtfword.cpp
sort_int.cpp
util.cpp
util_lst.cpp
util_spl.cpp
)

cif_visibility_hidden(cuneiform-rfrmt)
  
set_property(TARGET cuneiform-rfrmt PROPERTY COMPILE_DEFINITIONS __RFRMT__)

set(RFRMT_HEADER_INSTALL "rfrmt")
install(FILES formatoptions.h DESTINATION include/${CF_HEADER_INSTALL}/${RFRMT_HEADER_INSTALL})



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
