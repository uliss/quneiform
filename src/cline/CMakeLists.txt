set(CF_CLINE_SOURCE
    clinebase.cpp
    clinefunc.cpp
    clinemain.cpp
    debug.cpp
    lcline.cpp
)

add_library(${CF_CLINE} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_CLINE_SOURCE}
)

set_property(TARGET ${CF_CLINE} PROPERTY COMPILE_DEFINITIONS __CLINE__)

target_link_libraries(${CF_CLINE} ${CF_COMPAT})
cf_install_library(${CF_CLINE})
cf_cppcheck(${CF_CLINE})
