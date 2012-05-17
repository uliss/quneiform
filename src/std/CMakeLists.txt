add_library(cuneiform-std ${LIBTYPE}
std.cpp
stdbits.cpp
stdconsl.cpp
stderror.cpp
stdfile.cpp
stdiface.cpp
stdini.cpp
stdleo.cpp
stdprt.cpp
stdsort.cpp
)

cf_hide_visibility(cuneiform-std)

set_property(TARGET cuneiform-std PROPERTY COMPILE_DEFINITIONS __STD__)

target_link_libraries(cuneiform-std cuneiform-compat)

cf_install_library(cuneiform-std)
