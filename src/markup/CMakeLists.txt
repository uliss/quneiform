set(CF_MARKUP_SOURCE
    ${SNP_SRC}
    pagemarker.cpp
    shortverticallinesfilter.cpp
    svlprocessor.cpp
)

set(CF_MARKUP_HEADER
    pagemarker.h
    rmfunc.h
)

set(CF_MARKUP_LINK
    ${CF_CIMAGE}
    ${CF_CPAGE}
    cuneiform-rblock
    cuneiform-rline
    cuneiform-rneg
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-usage
)

add_library(${CF_MARKUP} ${LIBTYPE}
    ${CF_MARKUP_SOURCE}
    ${CF_MARKUP_HEADER}
)

cf_hide_visibility(${CF_MARKUP})
set_property(TARGET ${CF_MARKUP} PROPERTY COMPILE_DEFINITIONS __RMARKER__)
target_link_libraries(${CF_MARKUP} ${CF_MARKUP_LINK})
cf_install_library(${CF_MARKUP})
cf_cppcheck(${CF_MARKUP})
