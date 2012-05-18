set(CF_DIF_SOURCE
    dif.cpp
    diffr.cpp
    diffrb.cpp
    sticchar.cpp
    sticdif.cpp
    stictool.cpp
)

add_library(${CF_DIF} ${LIBTYPE} ${CF_DIF_SOURCE})
set_property(TARGET ${CF_DIF} PROPERTY COMPILE_DEFINITIONS __DIF__)

cf_install_library(${CF_DIF})
cf_cppcheck(${CF_DIF})
