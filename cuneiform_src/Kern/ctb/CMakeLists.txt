add_library(ctb32 ${LIBTYPE}
src/ctb_aln.cpp
src/ctb_cnvm.cpp
src/ctb_gray.cpp
src/ctb_oper.cpp
src/ctb_pack.cpp
src/ctb_prot.cpp
src/ctb_recr.cpp
src/ctb_tool.cpp
)
set_property(TARGET ctb32 PROPERTY COMPILE_DEFINITIONS __CTB__)

target_link_libraries(ctb32 cfcompat)

install_dll(ctb32)
