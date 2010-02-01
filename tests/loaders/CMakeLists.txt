add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(test_loaders
    test_loaders.cpp
    testformatdetector.cpp
    testbmp.cpp
)

target_link_libraries(test_loaders cuneiform-rdib ${CPPUNIT_LIBRARIES})