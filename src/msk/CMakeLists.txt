add_library(${CF_MSK} ${LIBTYPE} msk.cpp)
set_property(TARGET ${CF_MSK} PROPERTY COMPILE_DEFINITIONS __MSK__)

target_link_libraries(${CF_MSK}
    ${CF_MMX}
    ${CF_COMPAT})

cf_install_library(${CF_MSK})
cf_cppcheck(${CF_MSK})
