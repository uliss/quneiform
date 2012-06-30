set(CF_DIB_PLUGIN_SOURCES
    dibimageioplugin.cpp
    dibimageiohandler.cpp
)

set(CF_DIB_PLUGIN_HEADERS
    dibimageioplugin.h
    dibimageiohandler.h
)

add_definitions(-DQT_STATICPLUGIN)

add_library(dib_imageplugin STATIC
    ${CF_DIB_PLUGIN_SOURCES}
    ${CF_DIB_PLUGIN_HEADERS}
)

target_link_libraries(dib_imageplugin
    ${QT_LIBRARIES}
    cuneiform-rdib
)

if(CF_TIFF_SUPPORT)
    include_directories(${TIFF_INCLUDE_DIR})

    set(CF_MTIFF_PLUGIN_SOURCES
        multitiffiohandler.cpp
        multitiffioplugin.cpp
    )

    set(CF_MTIFF_PLUGIN_HEADERS
        multitiffiohandler.h
        multitiffioplugin.h
    )

    add_definitions(-DQT_STATICPLUGIN)

    add_library(multitiff_imageplugin STATIC
        ${CF_MTIFF_PLUGIN_SOURCES}
        ${CF_MTIFF_PLUGIN_HEADERS}
    )

    target_link_libraries(multitiff_imageplugin
        ${QT_LIBRARIES}
        ${TIFF_LIBRARIES}
    )
endif()

if(CF_PDF_SUPPORT)
    include_directories(${POPPLER_INCLUDE_DIR})

    set(CF_PDF_PLUGIN_SOURCES
        pdfiohandler.cpp
        pdfioplugin.cpp
    )

    set(CF_PDF_PLUGIN_HEADERS
        pdfiohandler.h
        pdfioplugin.h
    )

    add_definitions(-DQT_STATICPLUGIN)

    add_library(pdf_imageplugin STATIC
        ${CF_PDF_PLUGIN_SOURCES}
        ${CF_PDF_PLUGIN_HEADERS}
    )

    target_link_libraries(pdf_imageplugin
        ${QT_LIBRARIES}
        ${POPPLER_LIBRARY}
    )
endif()
