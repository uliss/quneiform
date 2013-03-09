add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
include(${QT_USE_FILE})

if(CF_PDF_SUPPORT)
    add_definitions(-DWITH_PDF)
endif()

if(CF_TIFF_SUPPORT)
    add_definitions(-DWITH_TIFF)
endif()

set(testloaders)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    include_directories(${ImageMagick_MagickCore_INCLUDE_DIR})
    list(APPEND testloaders testmagick.cpp)
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    list(APPEND testloaders testqtimageloader.cpp)
endif()

if(CF_USE_IMAGE_LIBRARY_FREEIMAGE)
    list(APPEND testloaders testfreeimageloader.cpp)
endif()

add_executable(test_loaders
    test_loaders.cpp
    testformatdetector.cpp
    testbmp.cpp
    ${testloaders}
)

target_link_libraries(test_loaders
    cuneiform-rdib 
    cuneiform
    ${CPPUNIT_LIBRARIES} 
    ${QT_LIBRARIES}
)

add_test(Loaders ${EXECUTABLE_OUTPUT_PATH}/test_loaders)
