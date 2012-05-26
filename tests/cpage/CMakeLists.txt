add_executable(test_cpage
    test_cpage.cpp
    testbackuppage.cpp
)

target_link_libraries(test_cpage
    cuneiform-cpage
    ${CPPUNIT_LIBRARIES}
)

add_test(Cpage ${EXECUTABLE_OUTPUT_PATH}/test_cpage)
