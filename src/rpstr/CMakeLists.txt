add_library(cuneiform-rpstr ${LIBTYPE}
${SNP_SRC}
cap_drop.cpp 
cor_incl.cpp
cor_spel.cpp 
rpstr.cpp 
vertstr.cpp)
set_property(TARGET cuneiform-rpstr PROPERTY COMPILE_DEFINITIONS __RPSTR__)

target_link_libraries(cuneiform-rpstr
    cuneiform-ccom
    cuneiform-cstr
    cuneiform-rling
    rstr
    std32
)

library_hook(cuneiform-rpstr)
