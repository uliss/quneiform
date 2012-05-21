set(CF_USAGE_SOURCE
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

set(CF_USAGE_HEADERS
    puma_err.h
    un_buff.h
    amt_geom.h
)

add_library(${CF_USAGE} ${LIBTYPE}
    ${CF_USAGE_SOURCE}
    ${USAGE_HEADERS}
)

target_link_libraries(${CF_USAGE}
    ${CF_CCOM}
    ${CF_CPAGE}
    ${CF_CLINE}
)

cf_install_library(${CF_USAGE})
cf_cppcheck(${CF_USAGE})
