include_directories(h)

add_library(puma ${LIBTYPE}
c/common.cpp 
c/debug.cpp 
c/enum.cpp
c/initdone.cpp 
c/out.cpp 
c/partlayout.cpp 
c/partrecog.cpp 
c/snp_sjtl.c 
main/mymem.cpp 
main/puma.cpp 
main/dll.cpp 
)

target_link_libraries(puma
ced
cfio
cimage
cpage
rblock
rcorrkegl
rfrmt
rimage
rline
rmarker
rout
rpic
rselstr
rshelllines
rstuff
exc
rreccom
rpstr
)

if(WIN32)
  install(TARGETS puma ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS puma LIBRARY DESTINATION ${LIBDIR})
endif()

