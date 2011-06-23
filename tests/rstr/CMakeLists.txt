add_executable(test_rstr
    test_rstr.cpp
    testbelarusian.cpp
)

set(TEST_RSTR_LIBS
    ${CPPUNIT_LIBRARIES}
    cuneiform-cstr
    cuneiform-fon
    cuneiform-leo
    cuneiform-cline
    cuneiform-rling
    cuneiform-pass2
    cuneiform-rcutp
    cuneiform-rbal
    cuneiform-rsadd
    cuneiform-rstr-static
)

target_link_libraries(test_rstr ${TEST_RSTR_LIBS})

add_test(RStr ${EXECUTABLE_OUTPUT_PATH}/test_rstr)
