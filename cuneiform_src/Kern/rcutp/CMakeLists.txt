include_directories(sources/main)

add_definitions(-D__RCUTP__)

add_library(rcutp ${LIBTYPE}
${SNP_SRC}
sources/main/rcut_4rselstr.cpp
#sources/main/rcutp.rc
sources/main/rcutp_call.cpp
sources/main/rcutp_func.cpp
sources/main/rcutp_main.cpp
sources/main/rcutp_mainfunc.cpp
)

target_link_libraries(rcutp ccom windummy)

if(WIN32)
  install(TARGETS rcutp ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rcutp LIBRARY DESTINATION ${LIBDIR})
endif()

