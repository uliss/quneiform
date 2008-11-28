include_directories(BEFORE
h
src
)

add_definitions(-DRSHELLLINES_EXPORTS)

add_library(rshelllines ${LIBTYPE}
${SNP_SRC}
src/rshelllines.cpp
#src/RShellLines.rc
src/stdafx.cpp
)

target_link_libraries(rshelllines
cfio
cimage
cpage
rverline
)

if(WIN32)
  install(TARGETS rshelllines ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rshelllines LIBRARY DESTINATION ${LIBDIR})
endif()
