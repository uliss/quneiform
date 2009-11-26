add_library(rneg ${LIBTYPE}
${SNP_SRC}
negatestcell.cpp
negdefs.cpp
negrecog.cpp
recnegmain.cpp
rnegbase.cpp
searchneg.cpp
)
set_property(TARGET rneg PROPERTY COMPILE_DEFINITIONS __RNEG__)

target_link_libraries(rneg
cimage
cpage
rdib
rimage
rselstr
exc
rstr
)

install_dll(rneg)
