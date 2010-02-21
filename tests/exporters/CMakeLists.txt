add_executable(test_exporters
    test_exporters.cpp
    testgenericexporter.cpp
    testhtmlexporter.cpp
)

target_link_libraries(test_exporters 
    cuneiform-common 
    cuneiform-ced 
    cuneiform-rfrmt 
    cuneiform-rout
    ${CPPUNIT_LIBRARIES})

add_test(Exporters ${EXECUTABLE_OUTPUT_PATH}/test_exporters)