add_executable(test_rselstr
    test_rselstr.cpp
    testrootlist.cpp
)

set(TEST_RSELSTR_LIBS
    ${CPPUNIT_LIBRARIES}
    cuneiform-rselstr
)

target_link_libraries(test_rselstr ${TEST_RSELSTR_LIBS})

add_test(RSelStr ${EXECUTABLE_OUTPUT_PATH}/test_rselstr)
