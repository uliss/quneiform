include_directories(BEFORE src/h)

add_library(rcorrkegl ${LIBTYPE}
src/cpp/cor_kegl.cpp
src/cpp/rcorrkegl.cpp
)
set_property(TARGET rcorrkegl PROPERTY COMPILE_DEFINITIONS __RCORRKEGL__)

target_link_libraries(rcorrkegl cstr cpage)

install_dll(rcorrkegl)
