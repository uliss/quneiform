set(image_exporters 
    imageexporter.cpp
    nullimageexporter.cpp
    bmpimageexporter.cpp)

if(USE_IMAGE_MAGICK) 
    set(image_exporters ${image_exporters} magickimageexporter.cpp)
    set(image_exporters_libs ${ImageMagick_Magick++_LIBRARY})
    message(STATUS "ImageMagick exporter will be build")
endif()

if(USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
endif()

# TODO, uliss: delete commented files
add_library(cuneiform-rout ${LIBTYPE}
exporter.cpp
exporterfactory.cpp
edexporter.cpp
debugexporter.cpp
imageexporterfactory.cpp
genericexporter.cpp
hocrexporter.cpp
htmlexporter.cpp
rtfexporter.cpp
textexporter.cpp
${image_exporters}
)

set_property(TARGET cuneiform-rout PROPERTY COMPILE_DEFINITIONS __ROUT__)

target_link_libraries(cuneiform-rout 
    cuneiform-ced 
    cuneiform-rfrmt
    ${image_exporters_libs})

library_hook(cuneiform-rout)
