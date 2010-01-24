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
sort_int.cpp
util.cpp
util_lst.cpp
util_spl.cpp
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-rfrmt PROPERTY COMPILE_FLAGS "-fvisibility=hidden") 
endif()
  
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

library_hook(cuneiform-rfrmt)
