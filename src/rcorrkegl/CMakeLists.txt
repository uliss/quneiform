add_library(rcorrkegl ${LIBTYPE}
${SNP_SRC}
cor_kegl.cpp
rcorrkegl.cpp
stdafx.cpp
)
set_property(TARGET rcorrkegl PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(rcorrkegl cstr cpage)

install_dll(rcorrkegl)
