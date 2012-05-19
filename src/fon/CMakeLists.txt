set(CF_FON_SOURCE
    clu_best.cpp
    clu_div.cpp
    clu_lang.cpp
    clu_test.cpp
    cutoff.cpp
    dist_bou.cpp
    distance.cpp
    fon_snap.cpp
    fonrec.cpp
    p2_addcl.cpp
    p2_clu.cpp
    p2_dsbit.cpp
    p2_mem.cpp
    p2_thick.cpp
    utilctb.cpp
)

add_library(${CF_FON} ${LIBTYPE} ${CF_FON_SOURCE})
set_property(TARGET ${CF_FON} PROPERTY COMPILE_DEFINITIONS __FON__)

target_link_libraries(${CF_FON}
    ${CF_CSTR}
    ${CF_CTB})

cf_install_library(${CF_FON})
cf_cppcheck(${CF_FON})
