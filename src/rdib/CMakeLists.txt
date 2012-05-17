set(loaders bmpimageloader.cpp)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND loaders magickimageloader.cpp)
    message(STATUS "Loaders: ImageMagick loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK)
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND loaders magickimageloader.cpp)
    message(STATUS "Loaders: GraphicsMagick loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    include_directories(${CF_IMAGE_INCLUDE})  
    include(${QT_USE_FILE})
    list(APPEND CF_IMAGE_LIBRARIES ${QT_LIBRARIES})
    list(APPEND loaders qtimageloader.cpp) 
    message(STATUS "Loaders: Qt loader will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_FREEIMAGE)
    if(MINGW)
        link_directories(${CMAKE_BINARY_DIR})
    endif()

    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND loaders freeimageloader.cpp)
    message(STATUS "Loaders: FreeImage loader will be build")
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

cf_install_library(cuneiform-rdib)

cf_install_headers(rdib
    imageloaderfactory.h
    imageloader.h
    qtimageloader.h
)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-rdib UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
