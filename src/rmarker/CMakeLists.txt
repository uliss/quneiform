add_library(rmarker ${LIBTYPE}
${SNP_SRC}
dll.cpp
rmarker.cpp
rmarkermem.cpp
shortverticallinesfilter.cpp
../usage/amu_geom.cpp
../usage/line_am_util.cpp
../usage/line_vp_2_am.cpp
../usage/line_vp_util.cpp
../usage/markdataoper.cpp
../usage/puma_err.cpp
../usage/un_buff.cpp
)

set_property(TARGET rmarker PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rmarker PROPERTY COMPILE_DEFINITIONS __RMARKER__)

target_link_libraries(rmarker
    cuneiform-cfio
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rblock
    cuneiform-rline
    rneg
    rpic
    rselstr
)

library_hook(rmarker)
