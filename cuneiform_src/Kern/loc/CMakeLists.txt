include_directories(BEFORE src)

add_library(loc32 ${LIBTYPE}
compdata.c
compkit.c
loc.c
locompmn.c
v0compgl.c
)                
set_property(TARGET loc32 PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(loc32 cfcompat)

install_dll(loc32)
