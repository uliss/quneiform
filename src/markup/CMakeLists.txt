set(MARKUP_SOURCES
    ${SNP_SRC}
    dll.cpp
    pagemarker.cpp
    shortverticallinesfilter.cpp
)

set(MARKUP_HEADERS
    pagemarker.h
    rmfunc.h
    markpage.h
)

add_library(cuneiform-markup ${LIBTYPE}
    ${MARKUP_SOURCES}
    ${MARKUP_HEADERS}
)

cif_visibility_hidden(cuneiform-markup)

set_property(TARGET cuneiform-markup PROPERTY COMPILE_DEFINITIONS __RMARKER__)

target_link_libraries(cuneiform-markup
    cuneiform-cfio
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rblock
    cuneiform-rline
    cuneiform-rneg
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-usage
)

library_hook(cuneiform-markup)
