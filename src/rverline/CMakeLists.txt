set(CF_RVERLINE_SOURCE
    ${SNP_SRC}
    amu_geom.cpp
    by_image.cpp
    pum_comm.cpp
    verifyln.cpp
    vl_kern.cpp
    vl_resid.cpp
    vl_data.cpp
    vl_rule.cpp
    vl_snap.cpp
)


add_library(${CF_RVERLINE} ${LIBTYPE} ${CF_RVERLINE_SOURCE})
cf_hide_visibility(${CF_RVERLINE})

set_property(TARGET ${CF_RVERLINE} PROPERTY COMPILE_DEFINITIONS __RVERLINE__)

target_link_libraries(${CF_RVERLINE}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_CLINE}
    ${CF_CCOM}
)

cf_install_library(${CF_RVERLINE})
cf_cppcheck(${CF_RVERLINE})
