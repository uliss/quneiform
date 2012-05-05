add_definitions(-DEXPORTER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

set(TEST_EXPORTER_SRC
    test_exporters.cpp
    testbmpimageexporter.cpp
    testcssexporter.cpp
    testgenericexporter.cpp
    testhtmlexporter.cpp
    testodfexporter.cpp
    teststyleexporter.cpp
    testtextexporter.cpp
    testzipcpp.cpp
)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    list(APPEND TEST_EXPORTER_SRC testmagickimageexporter.cpp)
endif()

if(CF_USE_IMAGE_LIBRARY_QT)
    list(APPEND TEST_EXPORTER_SRC testqtimageexporter.cpp)
endif()

add_executable(test_exporters
    ${TEST_EXPORTER_SRC}
)

target_link_libraries(test_exporters 
    cuneiform-common 
    cuneiform-ced 
    cuneiform-rfrmt 
    cuneiform-export
    ${CPPUNIT_LIBRARIES}
    ${CF_IMAGE_LIBRARIES}
)

add_test(Exporters ${EXECUTABLE_OUTPUT_PATH}/test_exporters)
