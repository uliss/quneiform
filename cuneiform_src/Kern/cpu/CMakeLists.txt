add_library(cpu ${LIBTYPE}
src/cpu.c
src/cpu32.c
)

install(TARGETS cpu LIBRARY DESTINATION lib)
