include_directories(BEFORE sources/main sources/main/deza sources/main/kronrod)

add_library(rimage ${LIBTYPE}
sources/main/deza/cdezabinarizator.cpp
sources/main/kronrod/ckronrodbinarizator.cpp
sources/main/kronrod/gdata.c
sources/main/kronrod/grey-wnd.c
sources/main/cribinarizator.cpp
sources/main/cricontrol.cpp
sources/main/criimage.cpp
#sources/main/crimage.rc
sources/main/crimemory.cpp
sources/main/crinvertor.cpp
sources/main/crrotator.cpp
sources/main/crturner.cpp
sources/main/dll.cpp
sources/main/rprogressor.cpp
)

target_link_libraries(rimage
cfio
cimage
rdib
)

install(TARGETS rimage LIBRARY DESTINATION lib)
