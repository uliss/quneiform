add_library(rshelllines ${LIBTYPE}
${SNP_SRC}
rshelllines.cpp
stdafx.cpp
)
set_property(TARGET rshelllines PROPERTY COMPILE_DEFINITIONS __RSHELLLINES_EXPORTS__)

target_link_libraries(rshelllines
cfio
cimage
cpage
rverline
)

install_dll(rshelllines)
