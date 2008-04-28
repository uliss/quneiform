include_directories(BEFORE
h
sources
../hhh/tigerh/h
)

add_library(rpic ${LIBTYPE}
${SNP_SRC}
sources/debug.cpp
sources/dll.cpp
sources/polypic.cpp
sources/rectpic.cpp
sources/rpic.cpp
)