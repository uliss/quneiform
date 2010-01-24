include_directories(BEFORE
../hhh/tigerh/h
)

add_library(cuneiform-rpic ${LIBTYPE}
${SNP_SRC}
debug.cpp
dll.cpp
polypic.cpp
rectpic.cpp
rpic.cpp
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-rpic PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
endif()

set_property(TARGET cuneiform-rpic PROPERTY COMPILE_DEFINITIONS __RPIC__)

target_link_libraries(cuneiform-rpic 
    cuneiform-cpage 
    cuneiform-ccom 
    cuneiform-rstr)

library_hook(cuneiform-rpic)
