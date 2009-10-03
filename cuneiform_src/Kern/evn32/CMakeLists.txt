include_directories( BEFORE  src )

add_library(evn32 ${LIBTYPE}
compdata.c
compkit.c
evn.c
locompmn.c
rastror.c
v0compan.c
v0comper.c
v0compev.c
v0compgl.c
v0comprq.c
)
set_property(TARGET evn32 PROPERTY COMPILE_DEFINITIONS __EVN__)

target_link_libraries(evn32 dif32 cfcompat)

install_dll(evn32)
