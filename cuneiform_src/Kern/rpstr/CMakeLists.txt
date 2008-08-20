include_directories(src)
include_directories(src/incl)
include_directories(src/capdrop)

add_library(rpstr ${LIBTYPE}
src/capdrop/cap_drop.c 
src/incl/cor_incl.c 
src/cor_spel.c 
src/rpstr.c 
src/vertstr.cpp)

target_link_libraries(rpstr
ccom
cstr
rling
rstr
std32
)

install(TARGETS rpstr LIBRARY DESTINATION ${LIBDIR})
