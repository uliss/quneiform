set(CF_MMX_SOURCE
    mmx_gra.cpp
    mmx_ind.cpp
    mmx_net.cpp
    mmx_r35.cpp
    mmxmain.cpp
)

add_library(${CF_MMX} ${LIBTYPE} ${CF_MMX_SOURCE})
set_property(TARGET ${CF_MMX} PROPERTY COMPILE_DEFINITIONS __MMX__)
cf_install_library(${CF_MMX})
