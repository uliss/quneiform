include_directories(BEFORE src)

add_library(loc ${LIBTYPE}
src/compdata.c
src/compkit.c
src/loc.c
src/locompmn.c
src/v0compgl.c
)