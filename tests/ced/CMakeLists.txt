add_executable(test_ced
    test_ced.cpp
    testcedchar.cpp
    testcedline.cpp
    testcedpicture.cpp
)

target_link_libraries(test_ced 
    cuneiform-ced
    ${Boost_SERIALIZATION_LIBRARY} 
    ${CPPUNIT_LIBRARIES})

add_test(CED ${EXECUTABLE_OUTPUT_PATH}/test_ced)