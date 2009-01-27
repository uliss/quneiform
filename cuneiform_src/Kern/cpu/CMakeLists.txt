add_definitions(-D__CPU__)

add_library(cpu ${LIBTYPE}
src/cpu.c
src/cpu32.c
)

install_dll(cpu)
