add_library(cuneiform-rneg ${LIBTYPE}
${SNP_SRC}
negatestcell.cpp
negdefs.cpp
negrecog.cpp
recnegmain.cpp
rnegbase.cpp
searchneg.cpp
)
set_property(TARGET cuneiform-rneg PROPERTY COMPILE_DEFINITIONS __RNEG__)

target_link_libraries(cuneiform-rneg
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rdib
    cuneiform-rimage
    rselstr
    cuneiform-exc
    rstr
)

library_hook(cuneiform-rneg)
