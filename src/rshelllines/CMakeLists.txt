add_library(cuneiform-rsl ${LIBTYPE}
${SNP_SRC}
rshelllines.cpp
)
set_property(TARGET cuneiform-rsl PROPERTY COMPILE_DEFINITIONS __RSHELLLINES_EXPORTS__)

target_link_libraries(cuneiform-rsl
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rverline
)

library_hook(cuneiform-rsl)
