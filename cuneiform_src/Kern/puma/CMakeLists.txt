include_directories(h)

add_library(puma ${LIBTYPE}
c/common.cpp 
c/debug.cpp 
c/enum.cpp
c/initdone.cpp 
c/out.cpp 
c/partlayout.cpp 
c/partrecog.cpp 
c/snp_sjtl.c 
main/mymem.cpp 
main/puma.cpp 
main/dll.cpp 
)