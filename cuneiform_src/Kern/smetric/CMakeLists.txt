include_directories(BEFORE src)

add_library(smetric ${LIBTYPE}
${SNP_SRC}
src/skewmain.cpp
src/sm_kern.cpp
src/ugol.cpp
../usage/amu_geom.cpp
../usage/gystogra.cpp
../usage/puma_comm.cpp
../usage/un_buff.cpp
)

target_link_libraries(smetric windummy)

install_dll(smetric)
