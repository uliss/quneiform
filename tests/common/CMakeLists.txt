add_executable(test_common
    test_common.cpp
    testcolor.cpp
    testlanguage.cpp
    testimagerawdata.cpp
    testpoint.cpp
    testrect.cpp
    testsize.cpp
    testoutputformat.cpp
)

target_link_libraries(test_common 
    cuneiform-common 
    ${CPPUNIT_LIBRARIES}
    ${Boost_SERIALIZATION_LIBRARY})

add_test(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)