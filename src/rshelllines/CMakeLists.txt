add_library(rshelllines ${LIBTYPE}
${SNP_SRC}
rshelllines.cpp
)
set_property(TARGET rshelllines PROPERTY COMPILE_DEFINITIONS __RSHELLLINES_EXPORTS__)

target_link_libraries(rshelllines
    cuneiform-cimage
    cuneiform-cpage
    rverline
)

library_hook(rshelllines)
