add_library(rsadd ${LIBTYPE} rsadd.cpp)

set_property(TARGET rsadd PROPERTY COMPILE_DEFINITIONS __RSADD__)

target_link_libraries(rsadd cstr)

library_hook(rsadd)
