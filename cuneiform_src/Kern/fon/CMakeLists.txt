add_library(fon32 ${LIBTYPE}
src/clu_best.cpp
src/clu_div.cpp
src/clu_lang.cpp
src/clu_test.cpp
src/cutoff.cpp
src/dist_bou.cpp
src/distance.cpp
src/fon_snap.cpp
src/fonrec.cpp
src/p2_addcl.cpp
src/p2_clu.cpp
src/p2_dsbit.cpp
src/p2_mem.cpp
src/p2_thick.cpp
src/utilctb.cpp
)
set_property(TARGET fon32 PROPERTY COMPILE_DEFINITIONS __FON__)

target_link_libraries(fon32 cstr ctb32)

install_dll(fon32)
