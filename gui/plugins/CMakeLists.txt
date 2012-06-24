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

find_package(TIFF)

if(TIFF_FOUND)
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
