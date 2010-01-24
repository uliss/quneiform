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

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-smetric PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
endif()
set_property(TARGET cuneiform-smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(cuneiform-smetric cuneiform-compat)

library_hook(cuneiform-smetric)
