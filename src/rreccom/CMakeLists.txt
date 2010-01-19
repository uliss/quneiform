add_library(rreccom ${LIBTYPE}
alphabet.cpp
componentrecognizer.cpp
rreccom.cpp
)

set_property(TARGET rreccom PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rreccom PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(rreccom 
    cuneiform-ccom 
    cuneiform-evn
    cuneiform-compat)

library_hook(rreccom)
