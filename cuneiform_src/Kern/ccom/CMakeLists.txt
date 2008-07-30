include_directories(src)

add_library(ccom ${LIBTYPE}
src/ccom.c
src/ccom_loc.c)

install(TARGETS ccom LIBRARY DESTINATION lib)
