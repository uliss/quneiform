add_library(rstuff ${LIBTYPE}
${SNP_SRC}
aboutlines.aboutlin.cpp
aboutlines.buffer.cpp
binarise.cpp
deletelines.cpp
dll.cpp
layout.cpp
linekiller.cpp
normalise.cpp
ocomp.cpp
ortomove.cpp
rsmemory.cpp
rstimer.cpp
rstuff.cpp
../usage/amu_geom.cpp
../usage/line_am_util.cpp
../usage/line_vp_2_am.cpp
../usage/line_vp_util.cpp
../usage/markdataoper.cpp
../usage/puma_err.cpp
../usage/un_buff.cpp
)

set_property(TARGET rstuff PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET rstuff PROPERTY COMPILE_DEFINITIONS __RSTUFF__)

target_link_libraries(rstuff
cfio
cimage
cpage
rdib
rimage
rline
smetric
rreccom
exc
)

install_dll(rstuff)
