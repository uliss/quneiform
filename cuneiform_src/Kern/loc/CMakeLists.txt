include_directories(BEFORE src)

add_library(loc32 ${LIBTYPE}
src/compdata.cpp
src/compkit.cpp
src/loc.cpp
src/locompmn.cpp
)                
set_property(TARGET loc32 PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(loc32 cfcompat)

install_dll(loc32)
