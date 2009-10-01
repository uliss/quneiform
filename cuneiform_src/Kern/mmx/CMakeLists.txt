include_directories(BEFORE src)

add_library(mmx32 ${LIBTYPE}
src/mmx_gra.c
src/mmx_ind.c
src/mmx_net.c
src/mmx_r35.c
src/mmxmain.c
)

set_property(TARGET mmx32 PROPERTY COMPILE_DEFINITIONS __MMX__)

install_dll(mmx32)
