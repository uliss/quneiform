include_directories(BEFORE src)

add_definitions(-D__DIF__)

add_library(dif ${LIBTYPE}
src/dif.c
src/diffr.c
src/diffrb.c
src/sticchar.c
src/sticdif.c
src/stictool.c
)

install_dll(dif)
