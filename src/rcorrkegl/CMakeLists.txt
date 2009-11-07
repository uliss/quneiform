add_library(rcorrkegl ${LIBTYPE}
cor_kegl.cpp
rcorrkegl.cpp
)
set_property(TARGET rcorrkegl PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(rcorrkegl cstr cpage)

install_dll(rcorrkegl)
