# This subdirectory needs to be built two different times.
# The latter one needs different compiler definitions.

include_directories(BEFORE sources sources/h)
add_subdirectory(sources)

add_definitions(-DRLING_EXPORTS)

add_library(rling ${LIBTYPE}
sources/cpp/crled.cpp
sources/cpp/crling.cpp
#SOURCE=.\CRLing.rc
sources/cpp/crlmemory.cpp
sources/cpp/dll.cpp
sources/cpp/rlcontrol.cpp
sources/c/rling_ma.c
sources/c/spel2dic.c
sources/c/spel2voc.c
sources/c/spelabc.c
sources/c/spelart.c
sources/c/spelbuf.c
sources/c/spelchk.c
sources/c/speldat1.c
sources/c/speldat2.c
sources/c/speldici.c
sources/c/speldict.c
sources/c/speldvoc.c
sources/c/speledf1.c
sources/c/speledf2.c
sources/c/speledio.c
sources/c/spelfun.c
sources/c/spelloop.c
sources/c/spelout.c
sources/c/spelq.c
sources/c/spelset.c
sources/c/spelspec.c
sources/c/udictini.c
sources/c/udictuti.c
)
set_property(TARGET rling PROPERTY COMPILE_DEFINITIONS __RLING__)

target_link_libraries(rling cstr rlings)

install_dll(rling)
