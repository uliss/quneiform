add_executable(test_exporters
    test_exporters.cpp
)

target_link_libraries(test_exporters cuneiform-common cuneiform-ced ${CPPUNIT_LIBRARIES})

add_test(Exporters ${EXECUTABLE_OUTPUT_PATH}/test_exporters)