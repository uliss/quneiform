add_library(smetric ${LIBTYPE}
${SNP_SRC}
skewmain.cpp
sm_kern.cpp
ugol.cpp
../usage/amu_geom.cpp
../usage/gystogra.cpp
../usage/puma_comm.cpp
../usage/un_buff.cpp
)

set_property(TARGET smetric PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(smetric cuneiform-cfcompat)

library_hook(smetric)
