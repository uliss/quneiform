# Find needed image libraries and includes
#
# This module defines:
# CF_IMAGE_LIBRARIES
# CF_IMAGE_INCLUDES
#

# by default all possible image libraries used
set(CF_USE_IMAGE_LIBRARY_QT True)
set(CF_USE_IMAGE_LIBRARY_GD True)
set(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK True)
set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK True)
set(CF_USE_IMAGE_LIBRARY_FREEIMAGE True)

# user can define variable CF_NO_QT to prevent using qt image library 
if(CF_NO_QT) 
    set(CF_USE_IMAGE_LIBRARY_QT False)
endif()

# user can define variable CF_NO_GD to prevent using gd image library
if(CF_NO_GD)
    set(CF_USE_IMAGE_LIBRARY_GD False)
endif()

# user can define variable CF_NO_GRAPHICS_MAGICK to prevent using graphicsmagick image library
if(CF_NO_GRAPHICS_MAGICK)
    set(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK False)
endif()

# user can define variable CF_NO_IMAGE_MAGICK to prevent using imagemagick image library
if(CF_NO_IMAGE_MAGICK)
    set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK False)
endif()

# user can define variable CF_NO_FREEIMAGE to prevent using FreeImage library
if(CF_NO_FREEIMAGE)
    set(CF_USE_IMAGE_LIBRARY_FREEIMAGE False)
endif()

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    # try to find ImageMagick++
    find_package(ImageMagick COMPONENTS Magick++)
    if(ImageMagick_Magick++_FOUND)
        message(STATUS "Image library: ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
        message(STATUS "Image library: ImageMagick++ library is ${ImageMagick_Magick++_LIBRARY}.")  
        list(APPEND CF_IMAGE_INCLUDE ${ImageMagick_Magick++_INCLUDE_DIR})
        list(APPEND CF_IMAGE_LIBRARIES ${ImageMagick_Magick++_LIBRARY})
    else()
        message(STATUS "Image library: ImageMagick++ not found.")
        set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK False)
    endif()
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    # try to find Qt
    set(QT_USE_QTPLUGIN TRUE)
    find_package(Qt4 4.4.1 COMPONENTS QtCore QtGui)
    if (QT_FOUND)
        message(STATUS "Image library: Qt found.")
        list(APPEND CF_IMAGE_INCLUDE ${QT_INCLUDE_DIR})
    else()
        message(STATUS "Image library: Qt not found.")
        set(CF_USE_IMAGE_LIBRARY_QT False)
    endif()
endif()

if(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK AND NOT CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    include(FindPkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(GM GraphicsMagick GraphicsMagick++)
        if(GM_FOUND)
            message(STATUS "Image library: GraphicsMagick++ found at  ${GM_GraphicsMagick++_LIBDIR}")
            link_directories(${GM_GraphicsMagick++_LIBDIR})
            list(APPEND CF_IMAGE_INCLUDE ${GM_GraphicsMagick++_INCLUDEDIR})
            list(APPEND CF_IMAGE_LIBRARIES ${GM_LIBRARIES})
        else()
            message(STATUS "Image library: GraphicsMagick++ not found.")
            set(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK False)
        endif()
    else()
        set(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK False)
    endif()
endif()

if(CF_USE_IMAGE_LIBRARY_FREEIMAGE)
    include(FindFreeImage)
    if(FREEIMAGE_FOUND)
        message(STATUS "Image library: FreeImage found at ${FREEIMAGE_INCLUDE_PATH}")
        list(APPEND CF_IMAGE_INCLUDE ${FREEIMAGE_INCLUDE_PATH})
        list(APPEND CF_IMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
    else()
        message(STATUS "Image library: FreeImage not found.")
        set(CF_USE_IMAGE_LIBRARY_FREEIMAGE False)
    endif()
endif()
