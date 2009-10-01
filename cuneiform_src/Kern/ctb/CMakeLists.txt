add_library(ctb32 ${LIBTYPE}
ctb_aln.c
ctb_cnvm.c
ctb_gray.c
ctb_oper.c
ctb_pack.c
ctb_prot.c
ctb_recr.c
ctb_tool.c
)
set_property(TARGET ctb32 PROPERTY COMPILE_DEFINITIONS __CTB__)

target_link_libraries(ctb32 cfcompat)

install_dll(ctb32)
