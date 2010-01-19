add_library(cline ${LIBTYPE}
${SNP_SRC}
clinebase.cpp
clinefunc.cpp
clinemain.cpp
debug.cpp
lcline.cpp
)

set_property(TARGET cline PROPERTY COMPILE_DEFINITIONS __CLINE__)

target_link_libraries(cline cuneiform-compat)

library_hook(cline)
