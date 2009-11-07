add_library(puma ${LIBTYPE}
common.cpp 
formatoptions.cpp
layoutoptions.cpp
pumaimpl.cpp
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

install_dll(puma)
