add_library(cuneiform-rreccom ${LIBTYPE}
componentrecognizer.cpp
)

#cif_visibility_hidden(cuneiform-rreccom)

set_property(TARGET cuneiform-rreccom PROPERTY COMPILE_DEFINITIONS __RRECCOM__)

target_link_libraries(cuneiform-rreccom 
    cuneiform-common
    cuneiform-ccom 
    cuneiform-evn
    cuneiform-alphabets
    cuneiform-compat)

library_hook(cuneiform-rreccom)
