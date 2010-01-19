add_library(cuneiform-loc ${LIBTYPE}
compdata.cpp
compkit.cpp
loc.cpp
locompmn.cpp
v0compgl.cpp
)        

set_property(TARGET cuneiform-loc PROPERTY COMPILE_FLAGS "-fvisibility=hidden")        
set_property(TARGET cuneiform-loc PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(cuneiform-loc cuneiform-compat)

library_hook(cuneiform-loc)
