add_definitions(-DLOADER_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

set(TEST_RIMAGE_SRC
    test_rimage.cpp
    testcricontrol.cpp
    testthresholdbinarizator.cpp
)

if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
    list(APPEND TEST_RIMAGE_SRC testhistogramcreator.cpp)
endif()

add_executable(test_rimage
    ${TEST_RIMAGE_SRC}
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

