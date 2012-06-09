set(CF_TEST_SCAN_SRC
    test_scan.cpp
)

if(CF_SANE_SUPPORT_BUILD)
    list(APPEND CF_TEST_SCAN_SRC testsanescanner.cpp)
endif()

add_executable(test_scan ${CF_TEST_SCAN_SRC})

set(EXTRA_LIB cuneiform-cimage)

target_link_libraries(test_scan
    cuneiform-scan
    ${CPPUNIT_LIBRARIES}
    ${EXTRA_LIB})

add_test(Scan ${EXECUTABLE_OUTPUT_PATH}/test_scan)
