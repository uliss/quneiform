set(image_exporters 
    imageexporter.cpp
    nullimageexporter.cpp
    bmpimageexporter.cpp)
    
set(exporters
    cssexporter.cpp
    exporter.cpp
    debugexporter.cpp
    djvutxtexporter.cpp
    djvuxmlexporter.cpp
    genericexporter.cpp
    hocrexporter.cpp
    htmlexporter.cpp
    textexporter.cpp
    smarttextexporter.cpp
    styleexporter.cpp
    summaryexporter.cpp
    xmlexporter.cpp
)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND image_exporters magickimageexporter.cpp)
    message(STATUS "Exporters: ImageMagick exporter will be build")
endif()

set(exporters_libs)

if(CF_USE_ZIP)
    include_directories(${ZIP_INCLUDE_DIR})
    list(APPEND exporters odfexporter.cpp odfstyleexporter.cpp)
    list(APPEND exporters_libs ${ZIP_LIBRARY})
    message(STATUS "Exporters: OpenDocument Format exporter will be build")
endif()

if(Boost_SERIALIZATION_FOUND)
    list(APPEND exporters cuneiformexporter.cpp)
    list(APPEND exporters_libs ${Boost_SERIALIZATION_LIBRARY})
    message(STATUS "Exporters: Cuneiform Format exporter will be build")
endif()

if(CF_USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
endif()

add_library(cuneiform-export ${LIBTYPE}
    exporterfactory.cpp
    imageexporterfactory.cpp
    ${exporters}
    ${image_exporters}
)

set_property(TARGET cuneiform-export PROPERTY COMPILE_DEFINITIONS __ROUT__)

target_link_libraries(cuneiform-export 
    cuneiform-ced 
    cuneiform-common
    cuneiform-rfrmt
    ${CF_IMAGE_LIBRARIES}
    ${exporters_libs})

library_hook(cuneiform-export)

set(EXPORT_HEADER_INSTALL "export")
install(FILES exporter.h imageexporter.h DESTINATION include/${CF_HEADER_INSTALL}/${EXPORT_HEADER_INSTALL})
