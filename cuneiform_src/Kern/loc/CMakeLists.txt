include_directories(BEFORE src)

add_library(loc ${LIBTYPE}
src/compdata.c
src/compkit.c
src/loc.c
src/locompmn.c
src/v0compgl.c
)
set_property(TARGET loc PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(loc windummy)

install_dll(loc)
