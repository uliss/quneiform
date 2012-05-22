set(CF_CIMAGE_SOURCE
    bitmask.cpp
    cticontrol.cpp
    ctiimage.cpp
    ctiimageheader.cpp
    ctiimagelist.cpp
    ctimask.cpp
    ctimaskline.cpp
    ctimasklinesegment.cpp
    ctimemory.cpp
    dll.cpp
)

set(CF_CIMAGE_INCLUDE
    imageinfo.h
)

add_library(${CF_CIMAGE} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_CIMAGE_SOURCE}
    ${CF_CIMAGE_INCLUDE}
)

set_property(TARGET ${CF_CIMAGE} PROPERTY COMPILE_DEFINITIONS __CIMAGE__)

target_link_libraries(${CF_CIMAGE}
    ${CF_RDIB}
    ${CF_COMMON}
)

cf_install_library(${CF_CIMAGE})
cf_install_headers(cimage imageinfo.h)
cf_cppcheck(${CF_CIMAGE})
