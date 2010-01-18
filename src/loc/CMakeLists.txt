add_library(loc32 ${LIBTYPE}
compdata.cpp
compkit.cpp
loc.cpp
locompmn.cpp
v0compgl.cpp
)        

set_property(TARGET loc32 PROPERTY COMPILE_FLAGS "-fvisibility=hidden")        
set_property(TARGET loc32 PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(loc32 cfcompat)

library_hook(loc32)
