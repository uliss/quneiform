add_library(${CF_RCORRKEGL} ${LIBTYPE}
    ${SNP_SRC}
    cor_kegl.cpp
    rcorrkegl.cpp
)

set_property(TARGET ${CF_RCORRKEGL} PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(${CF_RCORRKEGL}
    ${CF_CSTR}
    ${CF_CPAGE})

cf_install_library(${CF_RCORRKEGL})
cf_cppcheck(${CF_RCORRKEGL})
