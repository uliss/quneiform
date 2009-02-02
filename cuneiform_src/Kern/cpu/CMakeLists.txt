add_definitions(-D__CPU__)

add_library(cpu ${LIBTYPE}
src/cpu.c
src/cpu32.c
)
set_property(TARGET cpu PROPERTY COMPILE_DEFINITIONS __CPU__)

install_dll(cpu)
