add_executable(test_alphabets
    test_alphabets.cpp
    testalphabet.cpp
    testalphabetfactory.cpp
)

target_link_libraries(test_alphabets cuneiform-alphabets ${CPPUNIT_LIBRARIES})

add_test(Alphabets ${EXECUTABLE_OUTPUT_PATH}/test_alphabets)