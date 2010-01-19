add_library(cuneiform-r35 ${LIBTYPE}
r35.cpp)

set_property(TARGET cuneiform-r35 PROPERTY COMPILE_DEFINITIONS __R35__)

target_link_libraries(cuneiform-r35 
    cuneiform-mmx 
    cuneiform-std 
    cuneiform-compat)

library_hook(cuneiform-r35)
