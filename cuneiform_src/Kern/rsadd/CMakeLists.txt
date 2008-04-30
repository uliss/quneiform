include_directories(BEFORE src)

add_library(rsadd ${LIBTYPE} src/rsadd.c)

target_link_libraries(rsadd cstr)