add_definitions(-D__FON__)

add_library(fon ${LIBTYPE}
src/clu_best.c
src/clu_div.c
src/clu_lang.c
src/clu_test.c
src/cutoff.c
src/dist_bou.c
src/distance.c
src/fon_snap.c
src/fonrec.c
src/p2_addcl.c
src/p2_clu.c
src/p2_dsbit.c
src/p2_mem.c
src/p2_thick.c
src/utilctb.c
)

target_link_libraries(fon cstr ctb32)

if(WIN32)
  install(TARGETS fon ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS fon LIBRARY DESTINATION ${LIBDIR})
endif()
