add_executable(test_rfrmt
    test_rfrmt.cpp
    testcrtfchar.cpp
    testcrtffragment.cpp
    testcrtfstring.cpp
    testcrtfverticalcolumn.cpp
    testcrtfword.cpp
)

target_link_libraries(test_rfrmt
    ${CPPUNIT_LIBRARIES} 
    cuneiform
    cuneiform-rfrmt
)

add_test(Rfrmt ${EXECUTABLE_OUTPUT_PATH}/test_rfrmt)