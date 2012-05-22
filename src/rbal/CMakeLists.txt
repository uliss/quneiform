set(CF_RBAL_HEADERS
        bal.h
)
set(CF_RBAL_SOURCE
    gistogramma.cpp
    lin_ext.cpp
    lin_rec.cpp
    linbam.cpp
    linban.cpp
    linpon.cpp
    linutil.cpp
    statsearchbl.cpp
)

add_library(${CF_RBAL} ${LIBTYPE}
    ${CF_RBAL_SOURCE}
    ${CF_RBAL_HEADERS}
)
set_property(TARGET ${CF_RBAL} PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(${CF_RBAL}
    ${CF_CSTR}
    ${CF_EVN})

cf_install_library(${CF_RBAL})
cf_cppcheck(${CF_RBAL})
