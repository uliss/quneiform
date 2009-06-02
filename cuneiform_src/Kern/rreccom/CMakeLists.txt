include_directories(BEFORE src/h)

add_library(rreccom ${LIBTYPE}
src/cpp/alphabet.cpp
src/cpp/recog.cpp
src/cpp/rreccom.cpp
)
set_property(TARGET rreccom PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(rreccom ccom evn32 cfcompat)

install_dll(rreccom)
