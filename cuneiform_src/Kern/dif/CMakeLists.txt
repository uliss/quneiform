include_directories(BEFORE src)

add_library(dif32 ${LIBTYPE}
dif.cpp
diffr.cpp
diffrb.cpp
sticchar.cpp
sticdif.cpp
stictool.cpp
)
set_property(TARGET dif32 PROPERTY COMPILE_DEFINITIONS __DIF__)

install_dll(dif32)
