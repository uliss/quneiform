add_library(cuneiform ${NEWLIBTYPE}
${SNP_SRC}
pumaimpl.cpp
)

target_link_libraries(cuneiform
    common
    cuneiform-ced
    cuneiform-cimage
    cline
    cpage
    rblock
    rcorrkegl
    rfrmt
    rimage
    rline
    rmarker
    rout
    rpic
    rselstr
    rshelllines
    rstuff
    exc
    rreccom
    rpstr
)

if(LIBTYPE STREQUAL STATIC)
    target_link_libraries(cuneiform cifdebug)
endif()

library_hook(cuneiform)
