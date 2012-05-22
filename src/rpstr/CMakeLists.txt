set(CF_RPSTR_SOURCE
    cap_drop.cpp
    cor_incl.cpp
    cor_spel.cpp
    rpstr.cpp
    vertstr.cpp
)

add_library(${CF_RPSTR} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_RPSTR_SOURCE}
)
set_property(TARGET ${CF_RPSTR} PROPERTY COMPILE_DEFINITIONS __RPSTR__)

target_link_libraries(${CF_RPSTR}
    ${CF_CCOM}
    ${CF_CSTR}
    ${CF_RLING}
    ${CF_RSTR}
    ${CF_STD}
)

cf_install_library(${CF_RPSTR})
cf_cppcheck(${CF_RPSTR})
