add_library(rverline ${LIBTYPE}
${SNP_SRC}
amu_geom.cpp
by_image.cpp
pum_comm.cpp
verifyln.cpp
vl_kern.cpp
vl_resid.cpp
vl_data.cpp
vl_rule.cpp
vl_snap.cpp
)

set_property(TARGET rverline PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rverline PROPERTY COMPILE_DEFINITIONS __RVERLINE__)

target_link_libraries(rverline
cimage
cpage
cline
cuneiform-ccom
)

library_hook(rverline)
