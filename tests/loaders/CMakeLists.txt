add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    set(testmagick "testmagick.cpp")
endif()

add_executable(test_loaders
    test_loaders.cpp
    testformatdetector.cpp
    testbmp.cpp
    ${testmagick}
)

target_link_libraries(test_loaders
    cuneiform-rdib 
    ${CPPUNIT_LIBRARIES} 
    ${CF_IMAGE_LIBRARIES}
)

add_test(Loaders ${EXECUTABLE_OUTPUT_PATH}/test_loaders)