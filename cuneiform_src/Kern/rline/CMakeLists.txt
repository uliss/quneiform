include_directories(BEFORE sources)

add_library(rline ${LIBTYPE}
sources/dll.cpp
sources/findlostlines.cpp
sources/newline.cpp
sources/rline.cpp
)