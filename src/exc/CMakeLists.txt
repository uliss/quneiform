add_library(cuneiform-exc ${LIBTYPE}
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

set_property(TARGET cuneiform-exc PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET cuneiform-exc PROPERTY COMPILE_DEFINITIONS __EXC__)

target_link_libraries(cuneiform-exc 
    cuneiform-loc 
    cuneiform-ccom 
    cuneiform-evn)

library_hook(cuneiform-exc)
