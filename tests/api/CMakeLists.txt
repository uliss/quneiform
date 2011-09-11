include_directories(../../api)
add_definitions(-DTEST_IMG_PATH="${CMAKE_SOURCE_DIR}/images/quneiform")

add_executable(test_c_api
    test_c_api.cpp
    testcapi.cpp
)

target_link_libraries(test_c_api cuneiform-c cuneiform ${CPPUNIT_LIBRARIES})

add_test(CApi ${EXECUTABLE_OUTPUT_PATH}/test_c_api)
