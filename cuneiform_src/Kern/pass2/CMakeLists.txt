add_library(pass2 ${LIBTYPE}
p2_choic.c
p2_proc.c
p2_rstr.c
p2_test.c
p2_tools.c
)
set_property(TARGET pass2 PROPERTY COMPILE_DEFINITIONS __P2__)

target_link_libraries(pass2 cstr fon32 std32)

install_dll(pass2)
