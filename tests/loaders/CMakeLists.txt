add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if(USE_IMAGE_MAGICK)
    set(magicklib ${ImageMagick_Magick++_LIBRARY})
    set(testmagick "testmagick.cpp")
else()
    set(magicklib "")
    set(testmagick "")
endif()

add_executable(test_loaders
    test_loaders.cpp
    testformatdetector.cpp
    testbmp.cpp
    ${testmagick}
)

target_link_libraries(test_loaders cuneiform-rdib ${CPPUNIT_LIBRARIES} ${magicklib})