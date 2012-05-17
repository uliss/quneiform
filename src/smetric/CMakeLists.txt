add_library(cuneiform-smetric ${LIBTYPE}
    ${SNP_SRC}
    skewmain.cpp
    sm_kern.cpp
    ugol.cpp
)

cf_hide_visibility(cuneiform-smetric)

set_property(TARGET cuneiform-smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(cuneiform-smetric
    cuneiform-compat
    cuneiform-usage
)

cf_install_library(cuneiform-smetric)
