set(image_exporters 
    imageexporter.cpp
    nullimageexporter.cpp
    bmpimageexporter.cpp)

set(headers
    zipimpl.h)
    
set(exporters
    cssexporter.cpp
    exporter.cpp
    fb2exporter.cpp
    debugexporter.cpp
    djvutxtexporter.cpp
    djvuxmlexporter.cpp
    genericexporter.cpp
    hocrexporter.cpp
    htmlexporter.cpp
    odfexporter.cpp
    odfstyleexporter.cpp
    textexporter.cpp
    smarttextexporter.cpp
    styleexporter.cpp
    summaryexporter.cpp
    xmlexporter.cpp
    zipcpp.cpp
)

set(exporters_libs)

if(CF_USE_MINIZIP)
    include_directories("../../contrib")
    include_directories(${ZLIB_INCLUDE_DIR})
    list(APPEND exporters minizipimpl.cpp)
    list(APPEND exporters_libs minizip)
endif()

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) 
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND image_exporters magickimageexporter.cpp)
    message(STATUS "Exporters: ImageMagick exporter will be build")
elseif(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK)
    include_directories(${CF_IMAGE_INCLUDE})
    list(APPEND image_exporters magickimageexporter.cpp)
    message(STATUS "Exporters: GraphicsMagick exporter will be build")
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    include_directories(${CF_IMAGE_INCLUDE})
    include(${QT_USE_FILE})
    list(APPEND exporters qtimageexporter.cpp)
    list(APPEND exporter_lims ${QT_LIBRARIES})
    message(STATUS "Exporters: Qt image exporter will be build")
endif()

if(CF_SERIALIZE)
    list(APPEND exporters cuneiformexporter.cpp
        cuneiformbinexporter.cpp
        cuneiformxmlexporter.cpp)
    list(APPEND exporters_libs ${Boost_SERIALIZATION_LIBRARY})
    message(STATUS "Exporters: Cuneiform Format exporter will be build")
endif()

include_directories(${ICONV_INCLUDE_DIR})

add_library(cuneiform-export ${CF_EXPORT_LIBTYPE}
    exporterfactory.cpp
    imageexporterfactory.cpp
    ${headers}
    ${exporters}
    ${image_exporters}
)

target_link_libraries(cuneiform-export
    cuneiform-common
    cuneiform-ced 
    cuneiform-rfrmt
    ${CF_IMAGE_LIBRARIES}
    ${exporters_libs})

cf_install_library(cuneiform-export)

install_headers(export  
    exporter.h 
    imageexporter.h 
    exporterfactory.h)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-export UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
