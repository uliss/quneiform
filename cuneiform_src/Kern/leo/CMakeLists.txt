include_directories(BEFORE src)

add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(leo32 ${LIBTYPE}
${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
src/leo.c
src/leo_cont.c
src/leo_dll.c
src/leo_ind.c
src/leo_prn.c
src/leo_size.c
src/leo_stic.c
)
set_property(TARGET leo32 PROPERTY COMPILE_DEFINITIONS __LEO__)

target_link_libraries(leo32
cpu32
ctb32
dif32
evn32
fon32
msk32
r3532
std32
)

install_dll(leo32)
