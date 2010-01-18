# This subdirectory needs to be built two different times.
# The latter one needs different compiler definitions.

include_directories(BEFORE sources)
add_subdirectory(sources)

add_definitions(-DRLING_EXPORTS)

add_library(rling ${LIBTYPE}
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

set_property(TARGET rling PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rling PROPERTY COMPILE_DEFINITIONS __RLING__)

target_link_libraries(rling cstr rlings)

library_hook(rling)
