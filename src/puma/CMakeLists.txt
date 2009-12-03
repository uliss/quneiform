add_library(puma ${LIBTYPE}
${SNP_SRC}
common.cpp 
puma.cpp 
pumaimpl.cpp
formatoptions.cpp
)

set_property(TARGET puma PROPERTY COMPILE_DEFINITIONS __PUMA__)

target_link_libraries(puma
common
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

if(LIBTYPE STREQUAL STATIC)
    target_link_libraries(puma cifdebug)
endif()

install_dll(puma)
