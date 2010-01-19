add_library(cuneiform-rreccom ${LIBTYPE}
alphabet.cpp
componentrecognizer.cpp
rreccom.cpp
)

set_property(TARGET cuneiform-rreccom PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET cuneiform-rreccom PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(cuneiform-rreccom 
    cuneiform-ccom 
    cuneiform-evn
    cuneiform-compat)

library_hook(cuneiform-rreccom)
