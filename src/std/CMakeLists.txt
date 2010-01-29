add_library(cuneiform-std ${LIBTYPE}
std.cpp
stdassrt.cpp
stdbits.cpp
stdconsl.cpp
stderror.cpp
stdfile.cpp
stdiface.cpp
stdini.cpp
stdleo.cpp
stdprop.cpp
stdprt.cpp
stdsort.cpp
stdwapi.cpp
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND NOT MINGW)
  set_property(TARGET cuneiform-std PROPERTY COMPILE_FLAGS "-fvisibility=hidden")
endif()

set_property(TARGET cuneiform-std PROPERTY COMPILE_DEFINITIONS __STD__)

target_link_libraries(cuneiform-std cuneiform-compat)

library_hook(cuneiform-std)
