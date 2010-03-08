set(image_exporters 
    imageexporter.cpp
    nullimageexporter.cpp
    bmpimageexporter.cpp)
    
set(exporters
    edexporter.cpp
    exporter.cpp
    debugexporter.cpp
    genericexporter.cpp
    hocrexporter.cpp
    htmlexporter.cpp
    rtfexporter.cpp
    textexporter.cpp
    xmlexporter.cpp
)

if(USE_IMAGE_MAGICK) 
    set(image_exporters ${image_exporters} magickimageexporter.cpp)
    set(image_exporters_libs ${ImageMagick_Magick++_LIBRARY})
    message(STATUS "ImageMagick exporter will be build")
endif()

if(CF_USE_ZIP)
    include_directories(${ZIP_INCLUDE_DIR})
    set(exporters ${exporters} odfexporter.cpp)
    set(exporters_libs ${ZIP_LIBRARY})
    message(STATUS "OpenDocument Format exporter will be build")
endif()

if(USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
endif()

# TODO, uliss: delete commented files
add_library(cuneiform-export ${LIBTYPE}
    exporterfactory.cpp
    imageexporterfactory.cpp
    tag.cpp
    ${exporters}
    ${image_exporters}
)

set_property(TARGET cuneiform-export PROPERTY COMPILE_DEFINITIONS __ROUT__)

target_link_libraries(cuneiform-export 
    cuneiform-ced 
    cuneiform-common
    cuneiform-rfrmt
    ${image_exporters_libs}
    ${exporters_libs})

library_hook(cuneiform-export)
