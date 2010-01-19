add_library(cuneiform ${NEWLIBTYPE}
${SNP_SRC}
pumaimpl.cpp
)

target_link_libraries(cuneiform
    cuneiform-common
    cuneiform-ced
    cuneiform-cimage
    cuneiform-cline
    cuneiform-cpage
    cuneiform-rblock
    cuneiform-rcorrkegl
    cuneiform-rfrmt
    cuneiform-rimage
    cuneiform-rline
    cuneiform-rmarker
    cuneiform-rout
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-rsl
    rstuff
    cuneiform-exc
    cuneiform-rreccom
    cuneiform-rpstr
)

if(LIBTYPE STREQUAL STATIC)
    target_link_libraries(cuneiform cuneiform-dbg)
endif()

library_hook(cuneiform)
