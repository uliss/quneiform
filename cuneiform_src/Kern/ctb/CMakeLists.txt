add_library(ctb32 ${LIBTYPE}
src/ctb_aln.c
src/ctb_cnvm.c
src/ctb_gray.c
src/ctb_oper.c
src/ctb_pack.c
src/ctb_prot.c
src/ctb_recr.c
src/ctb_tool.c
)

target_link_libraries(ctb32 windummy)

if(WIN32)
  install(TARGETS ctb32 ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS ctb32 LIBRARY DESTINATION ${LIBDIR})
endif()
