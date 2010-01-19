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

set_property(TARGET cuneiform-rpic PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET cuneiform-rpic PROPERTY COMPILE_DEFINITIONS __RPIC__)

target_link_libraries(cuneiform-rpic 
    cuneiform-cpage 
    cuneiform-ccom 
    rstr)

library_hook(cuneiform-rpic)
