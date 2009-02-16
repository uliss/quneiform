include_directories(src)

add_library(msk32 ${LIBTYPE} src/msk.c)
set_property(TARGET msk32 PROPERTY COMPILE_DEFINITIONS __MSK__)

target_link_libraries(msk32 mmx32 windummy)

install_dll(msk32)
