add_definitions(-DTEST_IMG_PATH="${CMAKE_SOURCE_DIR}/images/quneiform")
add_definitions(-DTEST_EXE_PATH="${EXECUTABLE_OUTPUT_PATH}")

add_executable(test_puma
    test_puma.cpp
    testlocalrecognitionserver.cpp
    testpuma.cpp
    teststartprocess.cpp
    testprocessrecognitionserver.cpp
)

target_link_libraries(test_puma cuneiform ${CPPUNIT_LIBRARIES})

add_test(Puma ${EXECUTABLE_OUTPUT_PATH}/test_puma)
