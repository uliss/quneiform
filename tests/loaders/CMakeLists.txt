add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

set(testloaders)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    list(APPEND testloaders testmagick.cpp)
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    list(APPEND testloaders testqtimageloader.cpp)
endif()

if(CF_USE_IMAGE_LIBRARY_FREE_IMAGE)
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
    ${CF_IMAGE_LIBRARIES}
)

add_test(Loaders ${EXECUTABLE_OUTPUT_PATH}/test_loaders)
