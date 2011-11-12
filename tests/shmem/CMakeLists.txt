add_definitions(-DBOOST_DATE_TIME_NO_LIB)

add_executable(test_shmem
    test_shmem.cpp
    testsharedmemoryholder.cpp
    testmemorydata.cpp
)

set(TEST_SHMEM_LIBS
    ${CPPUNIT_LIBRARIES}
    cuneiform-shmem
)

target_link_libraries(test_shmem ${TEST_SHMEM_LIBS})

add_test(SharedMemory ${EXECUTABLE_OUTPUT_PATH}/test_shmem)
