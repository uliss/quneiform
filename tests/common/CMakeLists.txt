add_executable(test_common
    test_common.cpp
    testlanguage.cpp
    testpoint.cpp
    testrect.cpp
    testoutputformat.cpp
)

target_link_libraries(test_common cuneiform-common ${CPPUNIT_LIBRARIES})

add_test(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)