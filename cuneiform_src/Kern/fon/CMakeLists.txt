add_library(fon32 ${LIBTYPE}
clu_best.c
clu_div.c
clu_lang.c
clu_test.c
cutoff.c
dist_bou.c
distance.c
fon_snap.c
fonrec.c
p2_addcl.c
p2_clu.c
p2_dsbit.c
p2_mem.c
p2_thick.c
utilctb.c
)
set_property(TARGET fon32 PROPERTY COMPILE_DEFINITIONS __FON__)

target_link_libraries(fon32 cstr ctb32)

install_dll(fon32)
