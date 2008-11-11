include_directories(BEFORE src)

add_definitions(-DDPUMA_SNAP) # See warning below.

add_library(leo ${LIBTYPE}
${SNP_SRC} # WARNING: was not in the original dsp files, but I had to add this to make it link.
src/leo.c
src/leo_cont.c
src/leo_dll.c
src/leo_ind.c
src/leo_prn.c
src/leo_size.c
src/leo_stic.c
)

target_link_libraries(leo
cpu
ctb32
dif
evn32
fon
msk
r35
std32
)

if(WIN32)
  install(TARGETS leo ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS leo LIBRARY DESTINATION ${LIBDIR})
endif()
