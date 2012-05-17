add_library(cuneiform-rline ${LIBTYPE}
${SNP_SRC}
dll.cpp
findlostlines.cpp
newline.cpp
rline.cpp
)

cf_hide_visibility(cuneiform-rline)

set_property(TARGET cuneiform-rline PROPERTY COMPILE_DEFINITIONS __RLINE__)

target_link_libraries(cuneiform-rline
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-lns
    cuneiform-rsl
)

cf_install_library(cuneiform-rline)
