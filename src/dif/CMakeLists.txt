add_library(cuneiform-dif ${LIBTYPE}
dif.cpp
diffr.cpp
diffrb.cpp
sticchar.cpp
sticdif.cpp
stictool.cpp
)
set_property(TARGET cuneiform-dif PROPERTY COMPILE_DEFINITIONS __DIF__)

library_hook(cuneiform-dif)
