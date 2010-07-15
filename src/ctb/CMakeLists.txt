add_library(cuneiform-ctb ${LIBTYPE}
ctb_aln.cpp
ctb_cnvm.cpp
ctb_gray.cpp
ctb_oper.cpp
ctb_pack.cpp
ctb_recr.cpp
ctb_tool.cpp
)
set_property(TARGET cuneiform-ctb PROPERTY COMPILE_DEFINITIONS __CTB__)

target_link_libraries(cuneiform-ctb cuneiform-compat)

library_hook(cuneiform-ctb)
