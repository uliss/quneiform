add_library(r35 ${LIBTYPE}
src/r35.c)
set_property(TARGET r35 PROPERTY COMPILE_DEFINITIONS __R35__)

target_link_libraries(r35 mmx std32 windummy)

install_dll(r35)
