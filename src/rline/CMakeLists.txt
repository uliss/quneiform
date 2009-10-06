include_directories(BEFORE sources)

add_library(rline ${LIBTYPE}
${SNP_SRC}
sources/dll.cpp
sources/findlostlines.cpp
sources/newline.cpp
sources/rline.cpp
)
set_property(TARGET rline PROPERTY COMPILE_DEFINITIONS __RLINE__)

target_link_libraries(rline
cimage
cpage
lns32
rshelllines
)

install_dll(rline)
