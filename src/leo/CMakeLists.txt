add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(cuneiform-leo ${LIBTYPE}
${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
leo.cpp
leo_cont.cpp
leo_dll.cpp
leo_ind.cpp
leo_prn.cpp
leo_size.cpp
leo_stic.cpp
)
set_property(TARGET cuneiform-leo PROPERTY COMPILE_DEFINITIONS __LEO__)

target_link_libraries(cuneiform-leo
    cuneiform-cpu
    cuneiform-ctb    
    cuneiform-dif
    cuneiform-evn
    cuneiform-fon
    cuneiform-msk
    r3532
    std32
)

library_hook(cuneiform-leo)
