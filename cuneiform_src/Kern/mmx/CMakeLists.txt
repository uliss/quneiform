include_directories(BEFORE src)

add_library(mmx32 ${LIBTYPE}
src/mmx_gra.cpp
src/mmx_ind.cpp
src/mmx_net.cpp
src/mmx_r35.cpp
src/mmxmain.cpp
)

set_property(TARGET mmx32 PROPERTY COMPILE_DEFINITIONS __MMX__)

install_dll(mmx32)
