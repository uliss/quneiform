include_directories(src)

add_library(cstr ${LIBTYPE}
src/cgraph.cpp
src/cstr.c)

target_link_libraries(cstr ccom)