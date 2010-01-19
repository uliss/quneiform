add_library(cuneiform-rcorrkegl ${LIBTYPE}
${SNP_SRC}
cor_kegl.cpp
rcorrkegl.cpp
)
set_property(TARGET cuneiform-rcorrkegl PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(cuneiform-rcorrkegl 
    cuneiform-cstr 
    cuneiform-cpage)

library_hook(cuneiform-rcorrkegl)
