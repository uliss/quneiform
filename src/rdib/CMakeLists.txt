add_library(rdib ${LIBTYPE} ctdib.cpp)
set_property(TARGET rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)

install_dll(rdib)
