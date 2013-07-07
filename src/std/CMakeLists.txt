add_library(${CF_STD} ${LIBTYPE}
    std.cpp
    stdbits.cpp
    stdconsl.cpp
    stderror.cpp
    stdfile.cpp
    stdiface.cpp
    stdini.cpp
    stdleo.cpp
    stdprt.cpp
    stdsort.cpp
)

cf_hide_visibility(${CF_STD})

set_property(TARGET ${CF_STD} PROPERTY COMPILE_DEFINITIONS __STD__)

target_link_libraries(${CF_STD} ${CF_COMPAT} ${CF_COMMON})
cf_install_library(${CF_STD})
cf_cppcheck(${CF_STD})
