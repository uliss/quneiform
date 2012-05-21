set(CF_EXC_SOURCE
    compdata.cpp
    compkit.cpp
    componentextractor.cpp
    extrcomp.cpp
    locompmn.cpp
    matrix.cpp
    v0compan.cpp
    v0compgl.cpp
    v0compmn.cpp
)

add_library(${CF_EXC} ${LIBTYPE} ${CF_EXC_SOURCE})
cf_hide_visibility(${CF_EXC})
set_property(TARGET ${CF_EXC} PROPERTY COMPILE_DEFINITIONS __EXC__)

target_link_libraries(${CF_EXC}
    ${CF_LOC}
    ${CF_CCOM}
    ${CF_EVN})

cf_install_library(${CF_EXC})
cf_cppcheck(${CF_EXC})
