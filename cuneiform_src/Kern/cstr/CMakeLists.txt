include_directories(src)

add_library(cstr ${LIBTYPE}
src/cgraph.cpp
src/cstr.cpp)
set_property(TARGET cstr PROPERTY COMPILE_DEFINITIONS __CSTR__)

target_link_libraries(cstr ccom)

install_dll(cstr)
