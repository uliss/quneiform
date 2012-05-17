set(RBAL_HEADERS
        bal.h
)
set(RBAL_SOURCES
    gistogramma.cpp
    lin_ext.cpp
    lin_rec.cpp
    linbam.cpp
    linban.cpp
    linpon.cpp
    linutil.cpp
    statsearchbl.cpp
)

add_library(cuneiform-rbal ${LIBTYPE}
    ${RBAL_SOURCES}
    ${RBAL_HEADERS}
)
set_property(TARGET cuneiform-rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(cuneiform-rbal 
    cuneiform-cstr 
    cuneiform-evn)

cf_install_library(cuneiform-rbal)
