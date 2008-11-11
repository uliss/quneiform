include_directories(BEFORE src)

add_library(smetric ${LIBTYPE}
${SNP_SRC}
src/skewmain.cpp
src/sm_kern.cpp
src/ugol.cpp
../usage/amu_geom.cpp
../usage/gystogra.cpp
../usage/puma_comm.cpp
../usage/un_buff.cpp
)

target_link_libraries(smetric windummy)

if(WIN32)
  install(TARGETS smetric ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS smetric LIBRARY DESTINATION ${LIBDIR})
endif()

