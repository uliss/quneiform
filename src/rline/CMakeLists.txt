add_library(cuneiform-rline ${LIBTYPE}
${SNP_SRC}
dll.cpp
findlostlines.cpp
newline.cpp
rline.cpp
)

set_property(TARGET cuneiform-rline PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET cuneiform-rline PROPERTY COMPILE_DEFINITIONS __RLINE__)

target_link_libraries(cuneiform-rline
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-lns
    cuneiform-rsl
)

library_hook(cuneiform-rline)
