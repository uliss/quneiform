add_definitions(-D__CPU__)

add_library(${CF_CPU} ${LIBTYPE} cpu.cpp)

set_property(TARGET ${CF_CPU} PROPERTY COMPILE_DEFINITIONS __CPU__)
cf_install_library(${CF_CPU})
