include_directories(src)

add_library(std32 ${LIBTYPE}
src/std.cpp
#src/Std.rc
src/stdassrt.cpp
src/stdbits.cpp
src/stdconsl.cpp
src/stderror.cpp
src/stdfile.cpp
src/stdiface.cpp
src/stdini.cpp
src/stdleo.cpp
src/stdmem.cpp
src/stdprop.cpp
src/stdprt.cpp
src/stdsort.cpp
src/stdwapi.cpp
)
set_property(TARGET std32 PROPERTY COMPILE_DEFINITIONS __STD__)

target_link_libraries(std32 cfcompat)

install_dll(std32)
