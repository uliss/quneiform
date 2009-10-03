add_definitions(-D__CPU__)

add_library(cpu32 ${LIBTYPE}
src/cpu.cpp
src/cpu32.cpp
)

set_property(TARGET cpu32 PROPERTY COMPILE_DEFINITIONS __CPU__)

install_dll(cpu32)
