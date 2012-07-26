include_directories(BEFORE
../hhh/tigerh/h/
../hhh/tigerh/
)

set(CF_RSELSTR_SOURCE
    _adaptive.cpp
    _dlldummy.cpp
    _loadroots.cpp
    chstr_cutstr.cpp
    chstr_lhstr.cpp
    compdata.cpp
    cutcomp.cpp
    cutstr.cpp
    graphics.cpp
    ltblocks.cpp
    ltbreak.cpp
    lterrors.cpp
    lthyst.cpp
    ltinclin.cpp
    ltmain.cpp
    ltroots.cpp
    puttocont.cpp
    puttocstr.cpp
    rotate.cpp
    roots.cpp
    rselstrbase.cpp
    rselstrmain.cpp
    scalar.cpp
    seblocks.cpp
    seform.cpp
    sehbreak.cpp
    sehyst.cpp
    semain.cpp
    sestring.cpp
    sevbreak.cpp
    testforvertical.cpp
)

add_library(${CF_RSELSTR} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_RSELSTR_SOURCE}
)

cf_hide_visibility(${CF_RSELSTR})
set_property(TARGET ${CF_RSELSTR} PROPERTY COMPILE_DEFINITIONS __RSELSTR__)

target_link_libraries(${CF_RSELSTR}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_LOC}
    ${CF_RIMAGE}
    ${CF_EXC}
    ${CF_CLINE}
    ${CF_CSTR}
    ${CF_RCUTP}
)

cf_install_library(${CF_RSELSTR})
cf_cppcheck(${CF_RSELSTR})
