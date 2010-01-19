add_library(ctb32 ${LIBTYPE}
ctb_aln.cpp
ctb_cnvm.cpp
ctb_gray.cpp
ctb_oper.cpp
ctb_pack.cpp
ctb_prot.cpp
ctb_recr.cpp
ctb_tool.cpp
)
set_property(TARGET ctb32 PROPERTY COMPILE_DEFINITIONS __CTB__)

target_link_libraries(ctb32 cuneiform-compat)

library_hook(ctb32)
