include_directories(BEFORE src)

add_library(dif ${LIBTYPE}
src/dif.c
src/diffr.c
src/diffrb.c
src/sticchar.c
src/sticdif.c
src/stictool.c
)
set_property(TARGET dif PROPERTY COMPILE_DEFINITIONS __DIF__)

install_dll(dif)
