include_directories(BEFORE src)

add_library(dif32 ${LIBTYPE}
dif.c
diffr.c
diffrb.c
sticchar.c
sticdif.c
stictool.c
)
set_property(TARGET dif32 PROPERTY COMPILE_DEFINITIONS __DIF__)

install_dll(dif32)
