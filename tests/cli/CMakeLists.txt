add_executable(test_cli
    test_cli.cpp
    testoptions.cpp
    testoptionsparser.cpp
)

set(TEST_CLI_LIBS
    cuneiform-options
    cuneiform
    ${CPPUNIT_LIBRARIES}
)

target_link_libraries(test_cli 
    ${TEST_CLI_LIBS} 
)

add_test(Cli ${EXECUTABLE_OUTPUT_PATH}/test_cli)
