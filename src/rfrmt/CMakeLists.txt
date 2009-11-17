include_directories(BEFORE 
h
sources/main)

add_library(rfrmt ${LIBTYPE}
${SNP_SRC}
sources/main/crtfchar.cpp
sources/main/crtfword.cpp
sources/main/crtfstring.cpp
sources/main/crtffragment.cpp
sources/main/crtfsector.cpp
sources/main/crtfhorizontalcolumn.cpp
sources/main/crtfverticalcolumn.cpp
sources/main/crtfpage.cpp
sources/main/creatertf.cpp
sources/main/frmt.cpp
sources/main/frmtallignment.cpp
sources/main/frmtdibapi.cpp
sources/main/frmtdll.cpp
sources/main/frmtfile.cpp
sources/main/frmtpict.cpp
sources/main/int_file.cpp
sources/main/mymem.cpp
sources/main/pagetree.cpp
sources/main/sort_int.cpp
sources/main/util.cpp
sources/main/util_lst.cpp
sources/main/util_spl.cpp
)
set_property(TARGET rfrmt PROPERTY COMPILE_DEFINITIONS __RFRMT__)

target_link_libraries(rfrmt
ced
cimage
cpage
rdib
rimage
cline
cstr
cfcompat
)

install_dll(rfrmt)
