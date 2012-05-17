add_library(cuneiform-pass2 ${LIBTYPE}
p2_choic.cpp
p2_proc.cpp
p2_rstr.cpp
p2_test.cpp
p2_tools.cpp
)
set_property(TARGET cuneiform-pass2 PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(cuneiform-pass2 
    cuneiform-cstr 
    cuneiform-fon 
    cuneiform-std)

cf_install_library(cuneiform-pass2)
