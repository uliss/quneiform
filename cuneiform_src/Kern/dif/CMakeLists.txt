include_directories(BEFORE src)

add_library(dif32 ${LIBTYPE}
src/dif.cpp
src/diffr.cpp
src/diffrb.cpp
src/sticchar.cpp
src/sticdif.cpp
src/stictool.cpp
)
set_property(TARGET dif32 PROPERTY COMPILE_DEFINITIONS __DIF__)

install_dll(dif32)
