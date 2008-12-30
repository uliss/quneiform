include_directories(src)

add_definitions(-D__CCOM__) # Sets FUN_IMPO and FUN_EXPO properly.

add_library(ccom ${LIBTYPE}
src/ccom.c
src/ccom_loc.c)

install_dll(ccom)
