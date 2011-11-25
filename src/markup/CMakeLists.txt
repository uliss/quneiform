set(MARKUP_SOURCES
    ${SNP_SRC}
    dll.cpp
    pagemarker.cpp
    rmarkermem.cpp
    shortverticallinesfilter.cpp
    ../usage/amu_geom.cpp
    ../usage/line_am_util.cpp
    ../usage/line_vp_2_am.cpp
    ../usage/line_vp_util.cpp
    ../usage/markdataoper.cpp
    ../usage/un_buff.cpp
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
