add_library(r3532 ${LIBTYPE}
r35.cpp)

set_property(TARGET r3532 PROPERTY COMPILE_DEFINITIONS __R35__)

target_link_libraries(r3532 mmx32 std32 cfcompat)

library_hook(r3532)
