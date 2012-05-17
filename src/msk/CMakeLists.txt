add_library(cuneiform-msk ${LIBTYPE} msk.cpp)
set_property(TARGET cuneiform-msk PROPERTY COMPILE_DEFINITIONS __MSK__)

target_link_libraries(cuneiform-msk 
    cuneiform-mmx 
    cuneiform-compat)

cf_install_library(cuneiform-msk)
