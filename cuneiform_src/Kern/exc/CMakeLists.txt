include_directories(BEFORE src)

add_library(exc ${LIBTYPE}
src/compdata.cpp
src/compkit.cpp
src/extrcomp.cpp
src/locompmn.cpp
src/matrix.cpp
src/v0compan.cpp
src/v0compgl.cpp
src/v0compmn.cpp
)
set_property(TARGET exc PROPERTY COMPILE_DEFINITIONS __EXC__)

target_link_libraries(exc loc32 ccom evn32)

install_dll(exc)
