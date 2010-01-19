add_library(cuneiform-cstr ${LIBTYPE}
cgraph.cpp
cstr.cpp)
set_property(TARGET cuneiform-cstr PROPERTY COMPILE_DEFINITIONS __CSTR__)

target_link_libraries(cuneiform-cstr cuneiform-ccom)

library_hook(cuneiform-cstr)
