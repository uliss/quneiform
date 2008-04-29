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