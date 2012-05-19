add_definitions(-DDPUMA_SNAP) # See warning below.

set(CF_LNS_SOURCE
    bit_data.cpp
    extrlns.cpp
    fillgap3.cpp
    fltbuf.cpp
    frag.cpp
    hliner.cpp
    lbambuk.cpp
    lns.cpp
    lnscheck.cpp
    lnsini.cpp
    myassert.cpp
    procline.cpp
    puanso.cpp
    rbambuk.cpp
    rombabble.cpp
    sbambuk.cpp
    smooth.cpp
    sweeper.cpp
    tgreader.cpp
)

add_library(${CF_LNS} ${LIBTYPE}
    ${SNP_SRC} # WARNING: not here in original VS solution.
    ${CF_LNS_SOURCE}
)

set_property(TARGET ${CF_LNS} PROPERTY COMPILE_DEFINITIONS __LNS__)

target_link_libraries(${CF_LNS}
    ${CF_CLINE}
    cuneiform-std 
    ${CF_COMPAT})

cf_install_library(${CF_LNS})
cf_cppcheck(${CF_LNS})
