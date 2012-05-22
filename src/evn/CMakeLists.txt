set(CF_EVN_SOURCE
    compdata.cpp
    compkit.cpp
    evn.cpp
    linepool.cpp
    locompmn.cpp
    rastror.cpp
    v0compan.cpp
    v0comper.cpp
    v0compev.cpp
    v0compgl.cpp
    v0comprq.cpp
)

add_library(${CF_EVN} ${LIBTYPE} ${CF_EVN_SOURCE})
set_property(TARGET ${CF_EVN} PROPERTY COMPILE_DEFINITIONS __EVN__)
cf_hide_visibility(${CF_EVN})

target_link_libraries(${CF_EVN}
    ${CF_DIF}
    ${CF_COMPAT})

cf_install_library(${CF_EVN})
cf_cppcheck(${CF_EVN})
