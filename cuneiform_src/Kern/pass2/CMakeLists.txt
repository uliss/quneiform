include_directories(BEFORE src)

add_library(pass2 ${LIBTYPE}
src/p2_choic.c
src/p2_proc.c
src/p2_rstr.c
src/p2_test.c
src/p2_tools.c
)
set_property(TARGET pass2 PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(pass2 cstr fon32 std32)

install_dll(pass2)
