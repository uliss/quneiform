add_library(cuneiform-rcutp ${LIBTYPE}
${SNP_SRC}
rcut_4rselstr.cpp
rcutp_call.cpp
rcutp_func.cpp
rcutp_main.cpp
rcutp_mainfunc.cpp
)
set_property(TARGET cuneiform-rcutp PROPERTY COMPILE_DEFINITIONS __RCUTP__)

target_link_libraries(cuneiform-rcutp 
    cuneiform-ccom 
    cuneiform-compat)

cf_install_library(cuneiform-rcutp)
