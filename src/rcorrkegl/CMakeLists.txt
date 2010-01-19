add_library(rcorrkegl ${LIBTYPE}
${SNP_SRC}
cor_kegl.cpp
rcorrkegl.cpp
)
set_property(TARGET rcorrkegl PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(rcorrkegl 
    cstr 
    cuneiform-cpage)

library_hook(rcorrkegl)
