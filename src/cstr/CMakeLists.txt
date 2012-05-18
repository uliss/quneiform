add_library(${CF_CSTR} ${LIBTYPE}
    cgraph.cpp
    cstr.cpp
)

set_property(TARGET ${CF_CSTR} PROPERTY COMPILE_DEFINITIONS __CSTR__)

target_link_libraries(${CF_CSTR} ${CF_CCOM})
cf_install_library(${CF_CSTR})
cf_cppcheck(${CF_CSTR})
