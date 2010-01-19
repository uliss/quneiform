add_library(cuneiform-smetric ${LIBTYPE}
${SNP_SRC}
skewmain.cpp
sm_kern.cpp
ugol.cpp
../usage/amu_geom.cpp
../usage/gystogra.cpp
../usage/puma_comm.cpp
../usage/un_buff.cpp
)

set_property(TARGET cuneiform-smetric PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
set_property(TARGET cuneiform-smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(cuneiform-smetric cuneiform-compat)

library_hook(cuneiform-smetric)
