add_executable(test_puma
    test_puma.cpp
    testpuma.cpp
)

target_link_libraries(test_puma cuneiform ${CPPUNIT_LIBRARIES})

add_test(Puma ${EXECUTABLE_OUTPUT_PATH}/test_puma)