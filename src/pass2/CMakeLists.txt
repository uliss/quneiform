set(CF_PASS2_SOURCE
    p2_choic.cpp
    p2_proc.cpp
    p2_rstr.cpp
    p2_test.cpp
    p2_tools.cpp
)

add_library(${CF_PASS2} ${LIBTYPE} ${CF_PASS2_SOURCE})
set_property(TARGET ${CF_PASS2} PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(${CF_PASS2}
    ${CF_CSTR}
    ${CF_FON}
    ${CF_STD})

cf_install_library(${CF_PASS2})
cf_cppcheck(${CF_PASS2})
