add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    set(testloaders "testmagick.cpp")
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    set(testloaders ${testloaders} "testqtimageloader.cpp")
endif()

add_executable(test_loaders
    test_loaders.cpp
    testformatdetector.cpp
    testbmp.cpp
    ${testloaders}
)

target_link_libraries(test_loaders
    cuneiform-rdib 
    ${CPPUNIT_LIBRARIES} 
    ${CF_IMAGE_LIBRARIES}
)

add_test(Loaders ${EXECUTABLE_OUTPUT_PATH}/test_loaders)