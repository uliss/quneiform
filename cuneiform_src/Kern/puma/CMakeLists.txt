add_library(puma ${LIBTYPE}
${SNP_SRC}
common.cpp 
debug.cpp 
enum.cpp
initdone.cpp 
out.cpp 
partlayout.cpp 
partrecog.cpp 
snp_sjtl.c 
puma.cpp 
dll.cpp 
pumaimpl.cpp
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
