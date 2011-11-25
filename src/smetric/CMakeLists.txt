add_library(cuneiform-smetric ${LIBTYPE}
    ${SNP_SRC}
    skewmain.cpp
    sm_kern.cpp
    ugol.cpp
    ../usage/gystogra.cpp
)

cif_visibility_hidden(cuneiform-smetric)

set_property(TARGET cuneiform-smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(cuneiform-smetric
    cuneiform-compat
    cuneiform-usage
)

library_hook(cuneiform-smetric)
