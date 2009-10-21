add_library(rmarker ${LIBTYPE}
${SNP_SRC}
linesbuffer.cpp
dll.cpp
rmarker.cpp
rmtimer.cpp
shortverticallinesfilter.cpp
../usage/amu_geom.cpp
../usage/line_am_util.cpp
../usage/line_vp_2_am.cpp
../usage/line_vp_util.cpp
../usage/markdataoper.cpp
../usage/puma_err.cpp
../usage/un_buff.cpp
)
set_property(TARGET rmarker PROPERTY COMPILE_DEFINITIONS __RMARKER__)

target_link_libraries(rmarker
cfio
cimage
cpage
rblock
rline
rneg
rpic
rselstr
)

install_dll(rmarker)
