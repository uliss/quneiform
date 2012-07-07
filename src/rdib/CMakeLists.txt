set(CF_RDIB_SOURCE
    imageformatdetector.cpp
    imageloader.cpp
    imageloaderfactory.cpp
    bmpimageloader.cpp
)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND CF_RDIB_SOURCE magickimageloader.cpp)
    message(STATUS "Loaders: ImageMagick loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK)
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND CF_RDIB_SOURCE magickimageloader.cpp)
    message(STATUS "Loaders: GraphicsMagick loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    include_directories(${CF_IMAGE_INCLUDE})  
    include(${QT_USE_FILE})
    list(APPEND CF_IMAGE_LIBRARIES ${QT_LIBRARIES})
    list(APPEND CF_RDIB_SOURCE qtimageloader.cpp)
    if(CF_PDF_SUPPORT)
        set_property(SOURCE qtimageloader.cpp PROPERTY COMPILE_DEFINITIONS WITH_PDF)
    endif()
    message(STATUS "Loaders: Qt loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_FREEIMAGE)
    if(MINGW)
        link_directories(${CMAKE_BINARY_DIR})
    endif()

    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND CF_RDIB_SOURCE freeimageloader.cpp)
    message(STATUS "Loaders: FreeImage loader will be build")
endif()

add_library(${CF_RDIB} ${LIBTYPE} ${CF_RDIB_SOURCE})

set_property(TARGET ${CF_RDIB} PROPERTY COMPILE_DEFINITIONS __RDIB__)

target_link_libraries(${CF_RDIB}
    ${CF_COMMON}
    ${CF_COMPAT}
    ${CF_IMAGE_LIBRARIES}
)

cf_install_library(${CF_RDIB})

cf_install_headers(rdib
    imageloaderfactory.h
    imageloader.h
    qtimageloader.h
)

cf_cppcheck(${CF_RDIB})
