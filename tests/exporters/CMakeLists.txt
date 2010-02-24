add_definitions(-DEXPORTER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if(${USE_IMAGE_MAGICK})
    set(magick_test testmagickimageexporter.cpp)
endif()

add_executable(test_exporters
    test_exporters.cpp
    testbmpimageexporter.cpp
    testgenericexporter.cpp
    testhtmlexporter.cpp
    ${magick_test}
)

target_link_libraries(test_exporters 
    cuneiform-common 
    cuneiform-ced 
    cuneiform-rfrmt 
    cuneiform-rout
    ${CPPUNIT_LIBRARIES})

add_test(Exporters ${EXECUTABLE_OUTPUT_PATH}/test_exporters)