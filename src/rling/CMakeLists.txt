# This subdirectory needs to be built two different times.
# The latter one needs different compiler definitions.

set(CF_RLINGS cuneiform-rlings)

include_directories(BEFORE sources)
add_subdirectory(sources)

add_definitions(-DRLING_EXPORTS)

set(CF_RLING_SOURCE
    sources/crled.cpp
    sources/crling.cpp
    sources/crlmemory.cpp
    sources/dll.cpp
    sources/rlcontrol.cpp
    sources/rling_ma.cpp
    sources/spel2dic.cpp
    sources/spel2voc.cpp
    sources/spelabc.cpp
    sources/spelart.cpp
    sources/spelbuf.cpp
    sources/spelchk.cpp
    sources/speldat1.cpp
    sources/speldat2.cpp
    sources/speldici.cpp
    sources/speldict.cpp
    sources/speldvoc.cpp
    sources/speledf1.cpp
    sources/speledf2.cpp
    sources/speledio.cpp
    sources/spelfun.cpp
    sources/spelloop.cpp
    sources/spelout.cpp
    sources/spelq.cpp
    sources/spelset.cpp
    sources/spelspec.cpp
    sources/udictini.cpp
    sources/udictuti.cpp
)

add_library(${CF_RLING} ${LIBTYPE} ${CF_RLING_SOURCE})
cf_hide_visibility(${CF_RLING})
set_property(TARGET ${CF_RLING} PROPERTY COMPILE_DEFINITIONS __RLING__)

target_link_libraries(${CF_RLING}
    ${CF_CSTR}
    ${CF_RLINGS}
    ${CF_COMMON}
)

cf_install_library(${CF_RLING})
cf_cppcheck(${CF_RLING})
