add_library(${CF_RCUTP} ${LIBTYPE}
    ${SNP_SRC}
    rcut_4rselstr.cpp
    rcutp_call.cpp
    rcutp_func.cpp
    rcutp_main.cpp
    rcutp_mainfunc.cpp
)
set_property(TARGET ${CF_RCUTP} PROPERTY COMPILE_DEFINITIONS __RCUTP__)

target_link_libraries(${CF_RCUTP}
    ${CF_CCOM}
    ${CF_COMPAT})

cf_install_library(${CF_RCUTP})
cf_cppcheck(${CF_RCUTP})
