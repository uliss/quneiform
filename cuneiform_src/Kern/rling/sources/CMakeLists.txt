add_definitions(-DRLINGS_EXPORTS -DRLING_SECONDARY)

add_library(rlings ${LIBTYPE}
cpp/crled.cpp
cpp/crling.cpp
cpp/crlmemory.cpp
cpp/dll.cpp
cpp/rlcontrol.cpp
c/rling_ma.cpp
c/spel2dic.cpp
c/spel2voc.cpp
c/spelabc.cpp
c/spelart.cpp
c/spelbuf.cpp
c/spelchk.cpp
c/speldat1.cpp
c/speldat2.cpp
c/speldici.cpp
c/speldict.cpp
c/speldvoc.cpp
c/speledf1.cpp
c/speledf2.cpp
c/speledio.cpp
c/spelfun.cpp
c/spelloop.cpp
c/spelout.cpp
c/spelq.cpp
c/spelset.cpp
c/spelspec.cpp
c/udictini.cpp
c/udictuti.cpp
)
set_property(TARGET rlings PROPERTY COMPILE_DEFINITIONS RLING_SECONDARY)

target_link_libraries(rlings cstr cfcompat)

install_dll(rlings)
