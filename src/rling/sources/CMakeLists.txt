add_definitions(-DRLINGS_EXPORTS -DRLING_SECONDARY)

add_library(${CF_RLINGS} ${LIBTYPE}
    crled.cpp
    crling.cpp
    crlmemory.cpp
    dll.cpp
    rlcontrol.cpp
    rling_ma.cpp
    spel2dic.cpp
    spel2voc.cpp
    spelabc.cpp
    spelart.cpp
    spelbuf.cpp
    spelchk.cpp
    speldat1.cpp
    speldat2.cpp
    speldici.cpp
    speldict.cpp
    speldvoc.cpp
    speledf1.cpp
    speledf2.cpp
    speledio.cpp
    spelfun.cpp
    spelloop.cpp
    spelout.cpp
    spelq.cpp
    spelset.cpp
    spelspec.cpp
    udictini.cpp
    udictuti.cpp
)

cf_hide_visibility(${CF_RLINGS})

set_property(TARGET ${CF_RLINGS} PROPERTY COMPILE_DEFINITIONS RLING_SECONDARY)

target_link_libraries(${CF_RLINGS}
    ${CF_CSTR}
    ${CF_COMPAT})

cf_install_library(${CF_RLINGS})
