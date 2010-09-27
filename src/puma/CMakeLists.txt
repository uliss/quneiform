add_library(cuneiform ${LIBTYPE}
    pumaimpl.cpp
    ${SNP_SRC}
)

target_link_libraries(cuneiform
    cuneiform-common
    cuneiform-ced
    cuneiform-cimage
    cuneiform-cline
    cuneiform-cpage
    cuneiform-export
    cuneiform-rblock
    cuneiform-rcorrkegl
    cuneiform-rfrmt
    cuneiform-rimage
    cuneiform-rline
    cuneiform-rmarker
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-rsl
    cuneiform-rstuff
    cuneiform-exc
    cuneiform-rreccom
    cuneiform-rpstr
)

if(LIBTYPE STREQUAL STATIC)
    target_link_libraries(cuneiform cuneiform-dbg)
endif()

library_hook(cuneiform)

set(PUMA_HEADER_INSTALL "puma")
install(FILES puma.h pumaimpl.h DESTINATION include/${CF_HEADER_INSTALL}/${PUMA_HEADER_INSTALL})
