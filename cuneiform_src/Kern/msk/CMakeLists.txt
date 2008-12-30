include_directories(src)

add_library(msk ${LIBTYPE} src/msk.c)

target_link_libraries(msk mmx windummy)

install_dll(msk)
