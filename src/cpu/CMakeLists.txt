add_definitions(-D__CPU__)

add_library(cuneiform-cpu ${LIBTYPE}
cpu.cpp
)

set_property(TARGET cuneiform-cpu PROPERTY COMPILE_DEFINITIONS __CPU__)

library_hook(cuneiform-cpu)
