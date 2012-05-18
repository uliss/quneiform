add_library(${CF_COMPAT} ${LIBTYPE}
    cfcompat.cpp
)

set_property(TARGET ${CF_COMPAT} PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

cf_install_library(${CF_COMPAT})
cf_cppcheck(${CF_COMPAT})
