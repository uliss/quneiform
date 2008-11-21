include_directories(src)

add_definitions(-D__STD__) # Function import/export.

add_library(std32 ${LIBTYPE}
src/std.cpp
#src/Std.rc
src/stdassrt.cpp
src/stdbits.cpp
src/stdconsl.cpp
src/stderror.cpp
src/stdfile.cpp
src/stdiface.cpp
src/stdini.cpp
src/stdleo.cpp
src/stdmem.cpp
src/stdprop.cpp
src/stdprt.cpp
src/stdsort.cpp
src/stdwapi.cpp
)

target_link_libraries(std32 windummy)

if(WIN32)
  install(TARGETS std32 ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS std32 LIBRARY DESTINATION ${LIBDIR})
endif()
