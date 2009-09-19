include_directories(
# ../cuneiform_src/Kern/include
../cuneiform_src/Kern/h
# Kern/hh
../cuneiform_src/Kern/hhh
../cuneiform_src/Kern/hdebug
../cuneiform_src/Kern/hrk
../cuneiform_src/Kern/puma/h
# Kern/hrkint
# Kern/hrstr
# Kern/fon/src
# Kern/usage
)

add_definitions("-Wall")

# CMake does not currently have direct support for ImageMagick.
include(FindPkgConfig)
pkg_check_modules(MAGICK ImageMagick++)
if(MAGICK_FOUND)
    set(USE_MAGICK TRUE)
    include_directories(${MAGICK_INCLUDE_DIRS})
    link_directories(${MAGICK_LIBRARY_DIRS})
    message("Image magick found.")
endif()


#TARGET_LINK_LIBRARIES(cifocrtest cifocr ${MAGICK_LIBRARIES})

ADD_LIBRARY(libcifocr SHARED
    blockelement.cpp
    cifocr.cpp
    cifstring.cpp
    document.cpp
    image.cpp
    htmlexporter.cpp
    ocrengine.cpp
    page.cpp
    paragraph.cpp
    puma.cpp
    table.cpp
    textexporter.cpp
)

