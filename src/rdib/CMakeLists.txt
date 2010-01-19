set(loaders bmpimageloader.cpp)

if(USE_IMAGE_MAGICK) 
    set(loaders ${loaders} magickimageloader.cpp)
    set(loaders_libs ${ImageMagick_Magick++_LIBRARY})
    message(STATUS "ImageMagick loader will be build")
endif()

add_library(rdib ${LIBTYPE} 
ctdib.cpp
image.cpp
imageformatdetector.cpp
imageloader.cpp
imageloaderfactory.cpp
${loaders}
)

set_property(TARGET rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)
target_link_libraries(rdib cuneiform-common ${loaders_libs})

library_hook(rdib)
