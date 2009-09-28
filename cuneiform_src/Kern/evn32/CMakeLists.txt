include_directories( BEFORE  src )

add_library(evn32 ${LIBTYPE}
src/compdata.cpp
src/compkit.cpp
src/evn.cpp
src/locompmn.cpp
src/rastror.cpp
src/v0compan.cpp
src/v0comper.cpp
src/v0compev.cpp
src/v0compgl.cpp
src/v0comprq.cpp
)
set_property(TARGET evn32 PROPERTY COMPILE_DEFINITIONS __EVN__)

target_link_libraries(evn32 dif32 cfcompat)

install_dll(evn32)
