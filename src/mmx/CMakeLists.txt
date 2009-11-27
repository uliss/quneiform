add_library(mmx32 ${LIBTYPE}
mmx_gra.cpp
mmx_ind.cpp
mmx_net.cpp
mmx_r35.cpp
mmxmain.cpp
)

set_property(TARGET mmx32 PROPERTY COMPILE_DEFINITIONS __MMX__)

install_dll(mmx32)
