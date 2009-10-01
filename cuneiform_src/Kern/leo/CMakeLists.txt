include_directories(BEFORE src)

add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(leo32 ${LIBTYPE}
${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
src/leo.cpp
src/leo_cont.cpp
src/leo_dll.cpp
src/leo_ind.cpp
src/leo_prn.cpp
src/leo_size.cpp
src/leo_stic.cpp
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
