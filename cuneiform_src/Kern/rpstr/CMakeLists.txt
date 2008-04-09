include_directories(src)
include_directories(src/incl)
include_directories(src/capdrop)

add_library(rpstr 
src/capdrop/cap_drop.c 
src/incl/cor_incl.c 
src/cor_spel.c 
src/rpstr.c 
src/vertstr.cpp)