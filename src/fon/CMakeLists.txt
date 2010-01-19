add_library(cuneiform-fon ${LIBTYPE}
clu_best.cpp
clu_div.cpp
clu_lang.cpp
clu_test.cpp
cutoff.cpp
dist_bou.cpp
distance.cpp
fon_snap.cpp
fonrec.cpp
p2_addcl.cpp
p2_clu.cpp
p2_dsbit.cpp
p2_mem.cpp
p2_thick.cpp
utilctb.cpp
)
set_property(TARGET cuneiform-fon PROPERTY COMPILE_DEFINITIONS __FON__)

target_link_libraries(cuneiform-fon 
    cuneiform-cstr 
    cuneiform-ctb)

library_hook(cuneiform-fon)
