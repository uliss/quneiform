add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(test_rimage
    test_rimage.cpp
    testcricontrol.cpp
)

set(EXTRA_LIB "")

if(CF_SERIALIZE)
    set(EXTRA_LIB ${Boost_SERIALIZATION_LIBRARY})
    if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        list(APPEND EXTRA_LIB pthread)
    endif()
endif()

target_link_libraries(test_rimage
    cuneiform-rimage 
    ${CPPUNIT_LIBRARIES}
    ${EXTRA_LIB})

add_test(RImage ${EXECUTABLE_OUTPUT_PATH}/test_rimage)

