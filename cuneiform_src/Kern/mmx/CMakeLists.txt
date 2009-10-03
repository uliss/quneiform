include_directories(BEFORE src)

add_library(mmx32 ${LIBTYPE}
mmx_gra.c
mmx_ind.c
mmx_net.c
mmx_r35.c
mmxmain.c
)

set_property(TARGET mmx32 PROPERTY COMPILE_DEFINITIONS __MMX__)

install_dll(mmx32)
