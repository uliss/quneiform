include_directories(BEFORE
../hhh/tigerh/h
)

set(CF_RPIC_SOURCE
    debug.cpp
    dll.cpp
    polypic.cpp
    rectpic.cpp
    rpic.cpp
)

add_library(${CF_RPIC} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_RPIC_SOURCE}
)

cf_hide_visibility(${CF_RPIC})

set_property(TARGET ${CF_RPIC} PROPERTY COMPILE_DEFINITIONS __RPIC__)

target_link_libraries(${CF_RPIC}
    ${CF_CPAGE}
    ${CF_CCOM}
    cuneiform-rstr)

cf_install_library(${CF_RPIC})
cf_cppcheck(${CF_RPIC})
