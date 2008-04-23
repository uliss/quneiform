include_directories(BEFORE
h
src
)

add_library(rshelllines
#SOURCE=..\Hdebug\__Snp.c
src/rshelllines.cpp
#src/RShellLines.rc
src/stdafx.cpp
)