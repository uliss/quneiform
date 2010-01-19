add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(cuneiform-lns ${LIBTYPE}
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
set_property(TARGET cuneiform-lns PROPERTY COMPILE_DEFINITIONS __LNS__)

target_link_libraries(cuneiform-lns 
    cuneiform-cline 
    cuneiform-std 
    cuneiform-compat)

library_hook(cuneiform-lns)
