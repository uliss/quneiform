include_directories(h)

add_library(puma ${LIBTYPE}
${SNP_SRC}
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
set_property(TARGET puma PROPERTY COMPILE_DEFINITIONS __PUMA__)

target_link_libraries(puma
ced
cfio
cimage
cline
cpage
rblock
rcorrkegl
rfrmt
rimage
rline
rmarker
rout
rpic
rselstr
rshelllines
rstuff
exc
rreccom
rpstr
)

install_dll(puma)
