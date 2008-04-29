include_directories(BEFORE src)

add_library(exc ${LIBTYPE}
src/compdata.c
src/compkit.c
src/extrcomp.c
src/locompmn.c
src/matrix.c
src/v0compan.c
src/v0compgl.c
src/v0compmn.c
)