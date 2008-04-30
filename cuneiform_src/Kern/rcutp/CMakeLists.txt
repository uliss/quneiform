include_directories(sources/main)

add_library(rcutp ${LIBTYPE}
${SNP_SRC}
sources/main/rcut_4rselstr.cpp
sources/main/rcutp.rc
sources/main/rcutp_call.cpp
sources/main/rcutp_func.cpp
sources/main/rcutp_main.cpp
sources/main/rcutp_mainfunc.cpp
)

target_link_libraries(rcutp ccom windummy)