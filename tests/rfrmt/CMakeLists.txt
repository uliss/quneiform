add_executable(test_rfrmt
    test_rfrmt.cpp
    testcrtfchar.cpp
    testcrtffragment.cpp
    testcrtfhorizontalcolumn.cpp
    testcrtfstring.cpp
    testcrtfverticalcolumn.cpp
    testcrtfword.cpp
)

target_link_libraries(test_rfrmt
    cuneiform-rfrmt
    ${CPPUNIT_LIBRARIES} 
)

add_test(Rfrmt ${EXECUTABLE_OUTPUT_PATH}/test_rfrmt)
