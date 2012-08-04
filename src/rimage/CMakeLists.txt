set(CF_RIMAGE_SOURCE
    binarizatorfactory.cpp
    cdezabinarizator.cpp
    ckronrodbinarizator.cpp
    histogramcreator.cpp
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
    otsubinarizator.cpp
    rprogressor.cpp
    thresholdbinarizator.cpp
)

add_library(${CF_RIMAGE} ${LIBTYPE} ${CF_RIMAGE_SOURCE})

set_property(TARGET ${CF_RIMAGE} PROPERTY COMPILE_DEFINITIONS __RIMAGE__)

target_link_libraries(${CF_RIMAGE}
    ${CF_CIMAGE}
    ${CF_RDIB}
)

cf_install_library(${CF_RIMAGE})
cf_cppcheck(${CF_RIMAGE})
