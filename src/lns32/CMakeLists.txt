add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(lns32 ${LIBTYPE}
${SNP_SRC} # WARNING: not here in original VS solution.
bit_data.cpp
extrlns.cpp
fillgap3.cpp
fltbuf.cpp
frag.cpp
hliner.cpp
lbambuk.cpp
lns.cpp
lnscheck.cpp
lnsini.cpp
myassert.cpp
procline.cpp
puanso.cpp
rbambuk.cpp
rombabble.cpp
sbambuk.cpp
smooth.cpp
sweeper.cpp
tgreader.cpp
)
set_property(TARGET lns32 PROPERTY COMPILE_DEFINITIONS __LNS__)

target_link_libraries(lns32 cline std32 cfcompat)

install_dll(lns32)
