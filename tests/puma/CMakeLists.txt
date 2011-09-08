add_definitions(-DTEST_IMG_PATH="${CMAKE_SOURCE_DIR}/images/quneiform")
add_definitions(-DTEST_EXE_PATH="${EXECUTABLE_OUTPUT_PATH}")

add_executable(test_puma
    test_puma.cpp
    testlocalrecognitionserver.cpp
    testpuma.cpp
    testsharedimage.cpp
    testsharedimageholder.cpp
    testsharedresult.cpp
    testsharedresultholder.cpp
    teststartprocess.cpp
    testprocessrecognitionserver.cpp
)

add_executable(service_sharedimageholder servicesharedimageholder.cpp)
target_link_libraries(service_sharedimageholder cuneiform)
add_executable(service_sharedresultholder servicesharedresultholder.cpp)
target_link_libraries(service_sharedresultholder cuneiform)

add_dependencies(test_puma
    service_sharedimageholder
    service_sharedresultholder
)

target_link_libraries(test_puma cuneiform ${CPPUNIT_LIBRARIES})

add_test(Puma ${EXECUTABLE_OUTPUT_PATH}/test_puma)
