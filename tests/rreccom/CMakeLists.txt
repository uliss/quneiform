add_executable(test_rreccom 
    test_rreccom.cpp
    testcomponentrecognizer.cpp
)

target_link_libraries(test_rreccom cuneiform-rreccom ${CPPUNIT_LIBRARIES})