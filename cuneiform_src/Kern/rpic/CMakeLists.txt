include_directories(BEFORE
h
sources
../hhh/tigerh/h
)

add_library(rpic
#..\..\Hdebug\__Snp.c
sources/debug.cpp
sources/dll.cpp
sources/polypic.cpp
sources/rectpic.cpp
sources/rpic.cpp
)