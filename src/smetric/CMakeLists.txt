add_library(${CF_SMETRIC} ${LIBTYPE}
    ${SNP_SRC}
    skewmain.cpp
    sm_kern.cpp
    ugol.cpp
)

cf_hide_visibility(${CF_SMETRIC})

set_property(TARGET ${CF_SMETRIC} PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(${CF_SMETRIC}
    ${CF_COMPAT}
    ${CF_USAGE}
)

cf_install_library(${CF_SMETRIC})
cf_cppcheck(${CF_SMETRIC})
