set(CF_TEST_RDIB_SRC
    test_rdib.cpp
)

#list(APPEND CF_TEST_RDIB_SRC testctdib.cpp)

add_executable(test_rdib ${CF_TEST_RDIB_SRC})

target_link_libraries(test_rdib
    cuneiform-rdib
    cuneiform-cimage
    ${CPPUNIT_LIBRARIES})

add_test(RDib ${EXECUTABLE_OUTPUT_PATH}/test_rdib)
