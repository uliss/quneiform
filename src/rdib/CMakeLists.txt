set(loaders bmpimageloader.cpp)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    set(loaders ${loaders} magickimageloader.cpp)
    message(STATUS "Loaders: ImageMagick loader will be build")
endif()

add_library(cuneiform-rdib ${LIBTYPE} 
    ctdib.cpp
    image.cpp
    imageformatdetector.cpp
    imageloader.cpp
    imageloaderfactory.cpp
    ${loaders}
)

set_property(TARGET cuneiform-rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)
target_link_libraries(cuneiform-rdib 
    cuneiform-common 
    cuneiform-compat 
    ${CF_IMAGE_LIBS})

library_hook(cuneiform-rdib)

install(FILES 
    imageloaderfactory.h
    image.h
    imageloader.h
    imageformats.h
    DESTINATION include/${CF_HEADER_INSTALL}/rdib)
