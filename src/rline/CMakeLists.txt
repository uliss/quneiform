add_library(rline ${LIBTYPE}
${SNP_SRC}
dll.cpp
findlostlines.cpp
newline.cpp
rline.cpp
)

set_property(TARGET rline PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rline PROPERTY COMPILE_DEFINITIONS __RLINE__)

target_link_libraries(rline
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-lns
    rshelllines
)

library_hook(rline)
