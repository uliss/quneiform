set(loaders bmpimageloader.cpp)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    set(loaders ${loaders} magickimageloader.cpp)
    message(STATUS "Loaders: ImageMagick loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    include_directories(${CF_IMAGE_INCLUDE})  
    include(${QT_USE_FILE})
    set(CF_IMAGE_LIBRARIES ${CF_IMAGE_LIBRARIES} ${QT_LIBRARIES})  
    set(loaders ${loaders} qtimageloader.cpp)
    message(STATUS "Loaders: Qt loader will be build")
endif()

add_library(cuneiform-rdib ${LIBTYPE} 
    ctdib.cpp
    imageformatdetector.cpp
    imageloader.cpp
    imageloaderfactory.cpp
    ${loaders}
)

set_property(TARGET cuneiform-rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)

target_link_libraries(cuneiform-rdib 
    cuneiform-common 
    cuneiform-compat 
    ${CF_IMAGE_LIBRARIES}
)

library_hook(cuneiform-rdib)

install(FILES 
    imageloaderfactory.h
    imageloader.h
    imageformats.h
    qtimageloader.h
    DESTINATION include/${CF_HEADER_INSTALL}/rdib)
