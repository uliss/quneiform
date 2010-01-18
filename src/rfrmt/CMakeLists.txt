add_library(rfrmt ${LIBTYPE}
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
sort_int.cpp
util.cpp
util_lst.cpp
util_spl.cpp
)

set_property(TARGET rfrmt PROPERTY COMPILE_FLAGS "-fvisibility=hidden") 
set_property(TARGET rfrmt PROPERTY COMPILE_DEFINITIONS __RFRMT__)

target_link_libraries(rfrmt
cfcompat
common
ced
cimage
cpage
rdib
rimage
cline
cstr
)

library_hook(rfrmt)
