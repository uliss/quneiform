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
set_property(TARGET rpic PROPERTY COMPILE_DEFINITIONS __RPIC__)

target_link_libraries(rpic cpage ccom rstr)

install_dll(rpic)
