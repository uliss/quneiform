include_directories(BEFORE src)

add_definitions(-D__RSADD__)

add_library(rsadd ${LIBTYPE} src/rsadd.c)

target_link_libraries(rsadd cstr)

install_dll(rsadd)
