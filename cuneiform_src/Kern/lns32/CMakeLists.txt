include_directories(BEFORE src)

add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(lns32 ${LIBTYPE}
${SNP_SRC} # WARNING: not here in original VS solution.
src/bit_data.cpp
src/extrlns.cpp
src/fillgap3.cpp
src/fltbuf.cpp
src/frag.cpp
src/hliner.cpp
src/lbambuk.cpp
src/lns.cpp
src/lnscheck.cpp
src/lnsini.cpp
src/myassert.cpp
src/procline.cpp
src/puanso.cpp
src/rbambuk.cpp
src/rombabble.cpp
src/sbambuk.cpp
src/smooth.cpp
src/sweeper.cpp
src/tgreader.cpp
)
set_property(TARGET lns32 PROPERTY COMPILE_DEFINITIONS __LNS__)

target_link_libraries(lns32 cline std32 windummy)

install_dll(lns32)
