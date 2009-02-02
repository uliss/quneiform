include_directories(BEFORE sources)

add_library(rdib ${LIBTYPE} sources/cpp/CTDIB.cpp)
set_property(TARGET rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)

install_dll(rdib)
