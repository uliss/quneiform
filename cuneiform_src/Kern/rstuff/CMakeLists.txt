include_directories(BEFORE sources/main)

add_library(rstuff ${LIBTYPE}
${SNP_SRC}
sources/main/aboutlines.aboutlin.cpp
sources/main/aboutlines.buffer.cpp
sources/main/autotemplate.cpp
sources/main/binarise.cpp
sources/main/deletelines.cpp
sources/main/dll.cpp
sources/main/layout.cpp
sources/main/linekiller.cpp
sources/main/normalise.cpp
sources/main/ocomp.cpp
sources/main/ortomove.cpp
sources/main/rsmemory.cpp
sources/main/rstimer.cpp
sources/main/rstuff.cpp
#sources/main/rstuff.rc
../usage/amu_geom.cpp
../usage/line_am_util.cpp
../usage/line_vp_2_am.cpp
../usage/line_vp_util.cpp
../usage/markdataoper.cpp
../usage/puma_err.cpp
../usage/un_buff.cpp
)

target_link_libraries(rstuff
cfio
cimage
cpage
rdib
rimage
rline
smetric
rreccom
exc
)

if(WIN32)
  install(TARGETS rstuff ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rstuff LIBRARY DESTINATION ${LIBDIR})
endif()

