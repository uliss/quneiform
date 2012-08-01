add_definitions(-DTEST_DATADIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test_common
    test_common.cpp
    testcolor.cpp
    testenvpaths.cpp
    testhelper.cpp
    testhistogram.cpp
    testiconv.cpp
    testconfigparser.cpp
    testlanguage.cpp
    testlog.cpp
    testmodules.cpp
    testimage.cpp
    testimagerawdata.cpp
    testimageurl.cpp
    testpercentcounter.cpp
    testpoint.cpp
    testrect.cpp
    testsize.cpp
    testtostring.cpp
    testoutputformat.cpp
)

set(EXTRA_LIB "")

if(CF_SERIALIZE)
    set(EXTRA_LIB ${Boost_SERIALIZATION_LIBRARY})
    if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        list(APPEND EXTRA_LIB pthread)
    endif()
endif()

target_link_libraries(test_common 
    cuneiform-common 
    ${CPPUNIT_LIBRARIES}
    ${EXTRA_LIB})

add_test(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)
