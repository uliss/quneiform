set(HEADERS
    maccftype.h
)

set(SOURCES
    macapplicationhelp.cpp
    macbundle.cpp
    machelp.mm
    maclocale.mm
    macstring.mm
    macpool.mm
    macfullscreen.mm
    macopenfile.mm
    mactoolbar.mm
    mactoolbardelegate.mm
)

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})

add_library(quneiform_mac STATIC ${HEADERS} ${SOURCES})

set(APPLE_LIBS)
find_library(APPLE_CORE_FOUNDATION CoreFoundation "/")
list(APPEND APPLE_LIBS ${APPLE_CORE_FOUNDATION})
find_library(APPLE_APP_KIT AppKit "/")
list(APPEND APPLE_LIBS ${APPLE_APP_KIT})
target_link_libraries(quneiform_mac ${APPLE_LIBS} ${QT_LIBRARIES})
