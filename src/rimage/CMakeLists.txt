add_library(rimage ${LIBTYPE}
cdezabinarizator.cpp
ckronrodbinarizator.cpp
gdata.cpp
grey-wnd.cpp
cribinarizator.cpp
cricontrol.cpp
criimage.cpp
crimemory.cpp
crinvertor.cpp
crrotator.cpp
crturner.cpp
dll.cpp
rprogressor.cpp
)
set_property(TARGET rimage PROPERTY COMPILE_DEFINITIONS __RIMAGE__)

target_link_libraries(rimage
cfio
cimage
rdib
)

library_hook(rimage)
