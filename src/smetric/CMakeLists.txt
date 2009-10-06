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
set_property(TARGET smetric PROPERTY COMPILE_DEFINITIONS __SMETRIC__)

target_link_libraries(smetric cfcompat)

install_dll(smetric)
