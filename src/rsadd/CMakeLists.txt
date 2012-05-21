add_library(${CF_RSADD} ${LIBTYPE} rsadd.cpp)

set_property(TARGET ${CF_RSADD} PROPERTY COMPILE_DEFINITIONS __RSADD__)

target_link_libraries(${CF_RSADD} ${CF_CSTR})
cf_install_library(${CF_RSADD})
cf_cppcheck(${CF_RSADD})
