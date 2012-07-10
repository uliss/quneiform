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

