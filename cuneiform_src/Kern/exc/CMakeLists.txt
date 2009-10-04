include_directories(BEFORE src)

add_library(exc ${LIBTYPE}
compdata.c
compkit.c
extrcomp.c
locompmn.c
matrix.c
v0compan.c
v0compgl.c
v0compmn.c
)
set_property(TARGET exc PROPERTY COMPILE_DEFINITIONS __EXC__)

target_link_libraries(exc loc32 ccom evn32)

install_dll(exc)
