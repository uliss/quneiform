add_library(rfrmt ${LIBTYPE}
${SNP_SRC}
crtfchar.cpp
crtfword.cpp
crtfstring.cpp
crtffragment.cpp
crtfsector.cpp
crtfhorizontalcolumn.cpp
crtfverticalcolumn.cpp
crtfpage.cpp
creatertf.cpp
frmt.cpp
rfrmt.cpp
frmtallignment.cpp
frmtdibapi.cpp
frmtdll.cpp
frmtfile.cpp
frmtpict.cpp
int_file.cpp
pagetree.cpp
sort_int.cpp
util.cpp
util_lst.cpp
util_spl.cpp
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
