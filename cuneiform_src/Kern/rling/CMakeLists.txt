# This subdirectory needs to be built two different times.
# The latter one needs different compiler definitions.

include_directories(BEFORE sources sources/h)
add_subdirectory(sources)

add_definitions(-DRLING_EXPORTS)

add_library(rling ${LIBTYPE}
sources/cpp/crled.cpp
sources/cpp/crling.cpp
sources/cpp/crlmemory.cpp
sources/cpp/dll.cpp
sources/cpp/rlcontrol.cpp
sources/c/rling_ma.cpp
sources/c/spel2dic.cpp
sources/c/spel2voc.cpp
sources/c/spelabc.cpp
sources/c/spelart.cpp
sources/c/spelbuf.cpp
sources/c/spelchk.cpp
sources/c/speldat1.cpp
sources/c/speldat2.cpp
sources/c/speldici.cpp
sources/c/speldict.cpp
sources/c/speldvoc.cpp
sources/c/speledf1.cpp
sources/c/speledf2.cpp
sources/c/speledio.cpp
sources/c/spelfun.cpp
sources/c/spelloop.cpp
sources/c/spelout.cpp
sources/c/spelq.cpp
sources/c/spelset.cpp
sources/c/spelspec.cpp
sources/c/udictini.cpp
sources/c/udictuti.cpp
)
set_property(TARGET rling PROPERTY COMPILE_DEFINITIONS __RLING__)

target_link_libraries(rling cstr rlings)

install_dll(rling)
