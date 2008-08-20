include_directories(BEFORE
h
src
)

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

install(TARGETS rshelllines LIBRARY DESTINATION ${LIBDIR})
