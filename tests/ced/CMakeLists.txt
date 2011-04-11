add_executable(test_ced
    test_ced.cpp
    testcedchar.cpp
    testcedframe.cpp
    testcedline.cpp
    testcedpage.cpp
    testcedpicture.cpp
)

set(EXTRA_LIB "")

if(CF_SERIALIZE)
    set(EXTRA_LIB ${Boost_SERIALIZATION_LIBRARY})
endif()

target_link_libraries(test_ced 
    cuneiform-ced
    ${EXTRA_LIB} 
    ${CPPUNIT_LIBRARIES})

add_test(CED ${EXECUTABLE_OUTPUT_PATH}/test_ced)