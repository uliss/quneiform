add_executable(test_ced
    test_ced.cpp
)

target_link_libraries(test_ced cuneiform-common ${CPPUNIT_LIBRARIES})

add_test(CED ${EXECUTABLE_OUTPUT_PATH}/test_ced)