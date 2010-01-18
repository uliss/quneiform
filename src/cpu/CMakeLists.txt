add_definitions(-D__CPU__)

add_library(cpu32 ${LIBTYPE}
cpu.cpp
)

set_property(TARGET cpu32 PROPERTY COMPILE_DEFINITIONS __CPU__)

library_hook(cpu32)
