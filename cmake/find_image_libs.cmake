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
    set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK False)
endif()

# user can define variable CF_NO_IMAGE_MAGICK to prevent using imagemagick image library
if(CF_NO_IMAGE_MAGICK)
    set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK False)
endif()

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    # try to find ImageMagick++
    message(STATUS "Image library: try to find ImageMagick++")
    find_package(ImageMagick COMPONENTS Magick++)
    if(ImageMagick_Magick++_FOUND)
        message(STATUS "Image library: ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
        message(STATUS "Image library: ImageMagick++ library is ${ImageMagick_Magick++_LIBRARY}.")  
        set(CF_IMAGE_INCLUDE ${CF_IMAGE_INCLUDE} ${ImageMagick_Magick++_INCLUDE_DIR})
        set(CF_IMAGE_LIBRARIES ${CF_IMAGE_LIBRARIES} ${ImageMagick_Magick++_LIBRARY})
    else()
        message(STATUS "Image library: ImageMagick++ not found.")
        set(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK False)
    endif()
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    # try to find Qt        
    message(STATUS "Image library: try to find Qt.")
    find_package(Qt4 4.4.1 COMPONENTS QtCore QtGui)
    if (QT_FOUND)
        message(STATUS "Image library: Qt found.")
        #include(${QT_USE_FILE})
        set(CF_IMAGE_INCLUDE ${CF_IMAGE_INCLUDE} ${QT_INCLUDE_DIR})
        set(CF_IMAGE_LIBRARIES ${CF_IMAGE_LIBRARIES} ${QT_LIBRARIES})
    else()
        message(STATUS "Image library: Qt not found.")
        set(CF_USE_IMAGE_LIBRARY_QT False)
    endif()
endif()

