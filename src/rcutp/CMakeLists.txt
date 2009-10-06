include_directories(BEFORE sources/main)

add_library(rcutp ${LIBTYPE}
${SNP_SRC}
sources/main/rcut_4rselstr.cpp
#sources/main/rcutp.rc
sources/main/rcutp_call.cpp
sources/main/rcutp_func.cpp
sources/main/rcutp_main.cpp
sources/main/rcutp_mainfunc.cpp
)
set_property(TARGET rcutp PROPERTY COMPILE_DEFINITIONS __RCUTP__)

target_link_libraries(rcutp ccom cfcompat)

install_dll(rcutp)
