add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(test_cimage
    test_cimage.cpp
    testbitmask.cpp
    testcimagelist.cpp
    testcticontrol.cpp
)

set(EXTRA_LIB "")

if(CF_SERIALIZE)
    set(EXTRA_LIB ${Boost_SERIALIZATION_LIBRARY})
    if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        list(APPEND EXTRA_LIB pthread)
    endif()
endif()

target_link_libraries(test_cimage
    cuneiform-cimage 
    ${CPPUNIT_LIBRARIES}
    ${EXTRA_LIB})

add_test(CImage ${EXECUTABLE_OUTPUT_PATH}/test_cimage)

