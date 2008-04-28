include_directories(BEFORE
h
src
)

add_library(rshelllines ${LIBTYPE}
#SOURCE=..\Hdebug\__Snp.c
src/rshelllines.cpp
#src/RShellLines.rc
src/stdafx.cpp
)