include_directories(BEFORE src)

add_library(pass2 ${LIBTYPE}
src/p2_choic.cpp
src/p2_proc.cpp
src/p2_rstr.cpp
src/p2_test.cpp
src/p2_tools.cpp
)
set_property(TARGET pass2 PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(pass2 cstr fon32 std32)

install_dll(pass2)
