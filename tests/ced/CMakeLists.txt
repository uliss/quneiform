add_executable(test_ced
    test_ced.cpp
    testcedline.cpp
)

target_link_libraries(test_ced cuneiform-ced ${CPPUNIT_LIBRARIES})

add_test(CED ${EXECUTABLE_OUTPUT_PATH}/test_ced)