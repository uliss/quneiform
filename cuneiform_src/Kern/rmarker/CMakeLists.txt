add_definitions(-D__RMARKER__)

add_library(rmarker ${LIBTYPE}
${SNP_SRC}
dll.cpp
rmarker.cpp
rmarkermem.cpp
rmtimer.cpp
shortverticallinesfilter.cpp
../usage/amu_geom.cpp
../usage/line_am_util.cpp
../usage/line_vp_2_am.cpp
../usage/line_vp_util.cpp
../usage/markdataoper.cpp
../usage/puma_err.cpp
../usage/un_buff.cpp
)

target_link_libraries(rmarker
cfio
cimage
cpage
rblock
rline
rneg
rpic
rselstr
)

if(WIN32)
  install(TARGETS rmarker ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rmarker LIBRARY DESTINATION ${LIBDIR})
endif()

