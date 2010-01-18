add_library(rcutp ${LIBTYPE}
${SNP_SRC}
rcut_4rselstr.cpp
rcutp_call.cpp
rcutp_func.cpp
rcutp_main.cpp
rcutp_mainfunc.cpp
)
set_property(TARGET rcutp PROPERTY COMPILE_DEFINITIONS __RCUTP__)

target_link_libraries(rcutp ccom cfcompat)

library_hook(rcutp)
