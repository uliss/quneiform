include_directories(BEFORE . src)

add_library(rout ${LIBTYPE}
src/browse.cpp
src/codetables.cpp
src/html.cpp
src/pictures.cpp
src/rout.cpp
src/rout_dll.cpp
src/stdafx.cpp
src/tabledbf.cpp
src/tabletext.cpp
src/text.cpp
src/words.cpp
)