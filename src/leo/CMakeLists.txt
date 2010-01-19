add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(leo32 ${LIBTYPE}
${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
leo.cpp
leo_cont.cpp
leo_dll.cpp
leo_ind.cpp
leo_prn.cpp
leo_size.cpp
leo_stic.cpp
)
set_property(TARGET leo32 PROPERTY COMPILE_DEFINITIONS __LEO__)

target_link_libraries(leo32
    cuneiform-cpu
    cuneiform-ctb    
    cuneiform-dif
    evn32
    fon32
    msk32
    r3532
    std32
)

library_hook(leo32)
