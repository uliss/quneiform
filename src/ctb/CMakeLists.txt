set(CF_CTB_SOURCE
    ctb_aln.cpp
    ctb_cnvm.cpp
    ctb_gray.cpp
    ctb_oper.cpp
    ctb_pack.cpp
    ctb_recr.cpp
    ctb_tool.cpp
)

add_library(${CF_CTB} ${LIBTYPE} ${CF_CTB_SOURCE})
set_property(TARGET ${CF_CTB} PROPERTY COMPILE_DEFINITIONS __CTB__)

target_link_libraries(${CF_CTB} ${CF_COMPAT})
cf_install_library(${CF_CTB})
cf_cppcheck(${CF_CTB})
