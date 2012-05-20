add_library(${CF_R35} ${LIBTYPE} r35.cpp)

set_property(TARGET ${CF_R35} PROPERTY COMPILE_DEFINITIONS __R35__)

target_link_libraries(${CF_R35}
    ${CF_MMX}
    cuneiform-std 
    ${CF_COMPAT})

cf_install_library(${CF_R35})
cf_cppcheck(${CF_R35})
