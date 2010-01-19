add_library(fon32 ${LIBTYPE}
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
set_property(TARGET fon32 PROPERTY COMPILE_DEFINITIONS __FON__)

target_link_libraries(fon32 
    cuneiform-cstr 
    cuneiform-ctb)

library_hook(fon32)
