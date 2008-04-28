include_directories(BEFORE src)

add_library(smetric ${LIBTYPE}
#SOURCE=..\..\Hdebug\__Snp.c
src/skewmain.cpp
src/sm_kern.cpp
src/ugol.cpp
../usage/amu_geom.cpp
../usage/gystogra.cpp
../usage/puma_comm.cpp
../usage/un_buff.cpp
)