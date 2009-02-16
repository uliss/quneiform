include_directories(BEFORE src)

add_library(loc32 ${LIBTYPE}
src/compdata.c
src/compkit.c
src/loc.c
src/locompmn.c
src/v0compgl.c
)                
set_property(TARGET loc32 PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(loc32 windummy)

install_dll(loc32)
