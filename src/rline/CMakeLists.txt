set(CF_RLINE_SOURCE
    dll.cpp
    findlostlines.cpp
    newline.cpp
    rline.cpp
)

add_library(${CF_RLINE} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_RLINE_SOURCE}
)

cf_hide_visibility(${CF_RLINE})

set_property(TARGET ${CF_RLINE} PROPERTY COMPILE_DEFINITIONS __RLINE__)

target_link_libraries(${CF_RLINE}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_LNS}
    cuneiform-rsl
)

cf_install_library(${CF_RLINE})
cf_cppcheck(${CF_RLINE})
