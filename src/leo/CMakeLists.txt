add_definitions(-DDPUMA_SNAP) # See warning below.

set(CF_LEO_SOURCE
    leo.cpp
    leo_cont.cpp
    leo_dll.cpp
    leo_ind.cpp
    leo_prn.cpp
    leo_size.cpp
    leo_stic.cpp
)

set(CF_LEO_LINK
    cuneiform-cpu
    ${CF_CTB}
    ${CF_DIF}
    ${CF_EVN}
    ${CF_FON}
    cuneiform-msk
    cuneiform-r35
    cuneiform-std
)

add_library(${CF_LEO} ${LIBTYPE}
    ${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
    ${CF_LEO_SOURCE}
)

set_property(TARGET ${CF_LEO} PROPERTY COMPILE_DEFINITIONS __LEO__)

target_link_libraries(${CF_LEO} ${CF_LEO_LINK})
cf_install_library(${CF_LEO})
cf_cppcheck(${CF_LEO})
