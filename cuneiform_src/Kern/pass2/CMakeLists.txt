add_library(pass2 ${LIBTYPE}
p2_choic.cpp
p2_proc.cpp
p2_rstr.cpp
p2_test.cpp
p2_tools.cpp
)
set_property(TARGET pass2 PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(pass2 cstr fon32 std32)

install_dll(pass2)
