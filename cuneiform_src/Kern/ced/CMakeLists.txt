include_directories(h sources/h)

add_library(ced
sources/main/ced.cpp
sources/main/ced_func.cpp
sources/main/ced_func_old.cpp
sources/main/ced_func_rtf.cpp
sources/main/ced_struct_implem.cpp
sources/main/dll.cpp
sources/main/stdafx.cpp
sources/main/wrapper.cpp
)