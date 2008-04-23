include_directories(BEFORE src/h)

add_library(rcorrkegl
#SOURCE=..\..\Hdebug\__Snp.c
src/cpp/cor_kegl.cpp
src/cpp/rcorrkegl.cpp
src/cpp/stdafx.cpp
)