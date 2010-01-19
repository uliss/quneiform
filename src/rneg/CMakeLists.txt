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
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rdib
    rimage
    rselstr
    cuneiform-exc
    rstr
)

library_hook(rneg)
