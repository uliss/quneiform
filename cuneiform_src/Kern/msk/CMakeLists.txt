include_directories(src)

add_library(msk ${LIBTYPE} src/msk.c)
set_property(TARGET msk PROPERTY COMPILE_DEFINITIONS __MSK__)

target_link_libraries(msk mmx windummy)

install_dll(msk)
