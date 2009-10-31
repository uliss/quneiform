add_definitions(-D__CED__)

add_library(ced ${LIBTYPE}
ced.cpp
cedpage.cpp
ced_func.cpp
ced_func_old.cpp
ced_func_rtf.cpp
ced_struct_implem.cpp
dll.cpp
wrapper.cpp
)

target_link_libraries(ced cfio cfcompat)

install_dll(ced)
