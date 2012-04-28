set(RIMAGE_SOURCES
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
    ibinarizator.cpp
    oldbinarizator.cpp
    rprogressor.cpp
    thresholdbinarizator.cpp
)

add_library(cuneiform-rimage ${LIBTYPE}
    ${RIMAGE_SOURCES}
)

set_property(TARGET cuneiform-rimage PROPERTY COMPILE_DEFINITIONS __RIMAGE__)

target_link_libraries(cuneiform-rimage
    cuneiform-cimage
    cuneiform-rdib
)

library_hook(cuneiform-rimage)
