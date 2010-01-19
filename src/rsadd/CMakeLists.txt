add_library(cuneiform-rsadd ${LIBTYPE} rsadd.cpp)

set_property(TARGET cuneiform-rsadd PROPERTY COMPILE_DEFINITIONS __RSADD__)

target_link_libraries(cuneiform-rsadd cuneiform-cstr)

library_hook(cuneiform-rsadd)
