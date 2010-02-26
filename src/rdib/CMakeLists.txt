set(loaders bmpimageloader.cpp)
set(loader_libs "")

if(USE_IMAGE_MAGICK) 
    set(loaders ${loaders} magickimageloader.cpp)
    set(loaders_libs ${ImageMagick_Magick++_LIBRARY})
    message(STATUS "ImageMagick loader will be build")
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
    ${loaders_libs})

library_hook(cuneiform-rdib)

install(FILES 
    imageloaderfactory.h
    image.h
    imageloader.h
    imageformats.h
    DESTINATION include/${CF_HEADER_INSTALL}/rdib)
