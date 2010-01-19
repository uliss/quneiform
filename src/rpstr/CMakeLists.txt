add_library(rpstr ${LIBTYPE}
${SNP_SRC}
cap_drop.cpp 
cor_incl.cpp
cor_spel.cpp 
rpstr.cpp 
vertstr.cpp)
set_property(TARGET rpstr PROPERTY COMPILE_DEFINITIONS __RPSTR__)

target_link_libraries(rpstr
    cuneiform-ccom
    cuneiform-cstr
    rling
    rstr
    std32
)

library_hook(rpstr)
