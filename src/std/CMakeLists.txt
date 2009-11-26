add_library(std32 ${LIBTYPE}
std.cpp
stdassrt.cpp
stdbits.cpp
stdconsl.cpp
stderror.cpp
stdfile.cpp
stdiface.cpp
stdini.cpp
stdleo.cpp
stdmem.cpp
stdprop.cpp
stdprt.cpp
stdsort.cpp
stdwapi.cpp
)
set_property(TARGET std32 PROPERTY COMPILE_DEFINITIONS __STD__)

target_link_libraries(std32 cfcompat)

install_dll(std32)
