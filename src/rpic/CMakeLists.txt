include_directories(BEFORE
../hhh/tigerh/h
)

add_library(cuneiform-rpic ${LIBTYPE}
${SNP_SRC}
debug.cpp
dll.cpp
polypic.cpp
rectpic.cpp
rpic.cpp
)

cf_hide_visibility(cuneiform-rpic)

set_property(TARGET cuneiform-rpic PROPERTY COMPILE_DEFINITIONS __RPIC__)

target_link_libraries(cuneiform-rpic 
    cuneiform-cpage 
    cuneiform-ccom 
    cuneiform-rstr)

cf_install_library(cuneiform-rpic)
