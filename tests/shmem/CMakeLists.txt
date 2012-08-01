add_definitions(-DBOOST_DATE_TIME_NO_LIB)

set(TEST_SHMEM_SRC test_shmem.cpp
    testsharedmemoryholder.cpp
    testmemorydata.cpp
)

if(USE_MMAP_SHMEM)
    list(APPEND TEST_SHMEM_SRC testmmapsharedmemory.cpp)
endif()

add_executable(test_shmem
    ${TEST_SHMEM_SRC}
)

set(TEST_SHMEM_LIBS
    ${CPPUNIT_LIBRARIES}
    cuneiform-shmem
)

target_link_libraries(test_shmem ${TEST_SHMEM_LIBS})

add_test(SharedMemory ${EXECUTABLE_OUTPUT_PATH}/test_shmem)
