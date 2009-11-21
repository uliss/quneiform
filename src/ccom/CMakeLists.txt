include_directories(src)

add_definitions(-D__CCOM__) # Sets FUN_IMPO and FUN_EXPO properly.

add_library(ccom ${LIBTYPE}
ccom.cpp
ccom_loc.cpp)

install_dll(ccom)
