add_library(exc ${LIBTYPE}
compdata.cpp
compkit.cpp
componentextractor.cpp
extrcomp.cpp
locompmn.cpp
matrix.cpp
v0compan.cpp
v0compgl.cpp
v0compmn.cpp
)

set_property(TARGET exc PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET exc PROPERTY COMPILE_DEFINITIONS __EXC__)

target_link_libraries(exc 
    loc32 
    cuneiform-ccom 
    cuneiform-evn)

library_hook(exc)
