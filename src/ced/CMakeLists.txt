add_definitions(-D__CED__)

add_library(cuneiform-ced ${LIBTYPE}
ced.cpp
ced_func.cpp
ced_func_old.cpp
ced_func_rtf.cpp
ced_struct_implem.cpp
dll.cpp
wrapper.cpp
)

target_link_libraries(cuneiform-ced cfio cuneiform-compat)

library_hook(cuneiform-ced)
