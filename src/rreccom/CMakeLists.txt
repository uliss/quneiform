add_library(${CF_RRECCOM} ${LIBTYPE}
    componentrecognizer.cpp
)

#cf_hide_visibility(cuneiform-rreccom)

set_property(TARGET ${CF_RRECCOM} PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(${CF_RRECCOM}
    ${CF_COMMON}
    ${CF_CCOM}
    ${CF_EVN}
    ${CF_ALPHABETS}
    ${CF_COMPAT})

cf_install_library(${CF_RRECCOM})
cf_cppcheck(${CF_RRECCOM})
