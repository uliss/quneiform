include_directories(BEFORE src)

add_library(loc
src/compdata.c
src/compkit.c
src/loc.c
src/locompmn.c
src/v0compgl.c
)