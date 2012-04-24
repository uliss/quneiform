set(CIMAGE_SRC
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

set(CIMAGE_INCLUDE
    imageinfo.h
)

add_library(cuneiform-cimage ${LIBTYPE}
    ${SNP_SRC}
    ${CIMAGE_SRC}
    ${CIMAGE_INCLUDE}
)

set_property(TARGET cuneiform-cimage PROPERTY COMPILE_DEFINITIONS __CIMAGE__)

target_link_libraries(cuneiform-cimage 
    cuneiform-rdib 
    cuneiform-common 
)

library_hook(cuneiform-cimage)

install(FILES imageinfo.h DESTINATION include/${CF_HEADER_INSTALL}/cimage)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-cimage UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()
