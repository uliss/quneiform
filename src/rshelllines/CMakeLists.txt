add_library(${CF_RSL} ${LIBTYPE}
    ${SNP_SRC}
    rshelllines.cpp
)
set_property(TARGET ${CF_RSL} PROPERTY COMPILE_DEFINITIONS __RSHELLLINES_EXPORTS__)

target_link_libraries(${CF_RSL}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_RVERLINE}
)

cf_install_library(${CF_RSL})
cf_cppcheck(${CF_RSL})
