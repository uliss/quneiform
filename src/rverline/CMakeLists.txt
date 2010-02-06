add_library(cuneiform-rverline ${LIBTYPE}
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

cif_visibility_hidden(cuneiform-rverline)

set_property(TARGET cuneiform-rverline PROPERTY COMPILE_DEFINITIONS __RVERLINE__)

target_link_libraries(cuneiform-rverline
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-cline
    cuneiform-ccom
)

library_hook(cuneiform-rverline)
