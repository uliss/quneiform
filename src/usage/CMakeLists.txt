set(USAGE_SOURCES
    ${SNP_SRC}
    puma_err.cpp
    un_buff.cpp
    puma_comm.cpp
    markdataoper.cpp
    amu_geom.cpp
    gystogra.cpp
    line_am_util.cpp
    line_vp_2_am.cpp
    line_vp_util.cpp
)

set(USAGE_HEADERS
    puma_err.h
    un_buff.h
    amt_geom.h
)

add_library(cuneiform-usage ${LIBTYPE}
    ${USAGE_SOURCES}
    ${USAGE_HEADERS}
)

target_link_libraries(cuneiform-usage
    cuneiform-ccom
    cuneiform-cpage
    cuneiform-cline
)

cf_install_library(cuneiform-usage)
