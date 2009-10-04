add_library(rpstr ${LIBTYPE}
${SNP_SRC}
cap_drop.c 
cor_incl.c 
cor_spel.c 
rpstr.c 
vertstr.cpp)
set_property(TARGET rpstr PROPERTY COMPILE_DEFINITIONS __RPSTR__)

target_link_libraries(rpstr
ccom
cstr
rling
rstr
std32
)

install_dll(rpstr)
