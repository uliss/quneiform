add_library(cuneiform-cline ${LIBTYPE}
${SNP_SRC}
clinebase.cpp
clinefunc.cpp
clinemain.cpp
debug.cpp
lcline.cpp
)

set_property(TARGET cuneiform-cline PROPERTY COMPILE_DEFINITIONS __CLINE__)

target_link_libraries(cuneiform-cline cuneiform-compat)

library_hook(cuneiform-cline)
