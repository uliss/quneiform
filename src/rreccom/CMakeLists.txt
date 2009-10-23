add_library(rreccom ${LIBTYPE}
alphabet.cpp
recog.cpp
rreccom.cpp
)
set_property(TARGET rreccom PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(rreccom ccom evn32 cfcompat)

install_dll(rreccom)
