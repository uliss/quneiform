add_executable(test_cpage
    test_cpage.cpp
    testbackuppage.cpp
    testblock.cpp
    testcommondata.cpp
    testconvert.cpp
    testcpicture.cpp
    testdata.cpp
    testpage.cpp
    testpagestorage.cpp
    testpicture.cpp
    testpolyblock.cpp
)

set(TEST_CPAGE_LIBS cuneiform-cpage ${CPPUNIT_LIBRARIES})

if(WITH_COVERAGE)
    list(APPEND TEST_CPAGE_LIBS gcov)
endif()

target_link_libraries(test_cpage ${TEST_CPAGE_LIBS})
add_test(Cpage ${EXECUTABLE_OUTPUT_PATH}/test_cpage)
add_test_lcov(cpage)

