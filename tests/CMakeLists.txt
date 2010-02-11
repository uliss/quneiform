include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/h)
include_directories(../src/include)
include_directories(../src/hrstr)

find_package(CppUnit)
if(CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    message(STATUS "CppUnit include directory: " ${CPPUNIT_INCLUDE_DIR})
    add_executable(test_alphabets
        alphabets/test_alphabets.cpp
        alphabets/testalphabet.cpp
        alphabets/testalphabetfactory.cpp
    )

    add_executable(test_common
        common/test_common.cpp
        common/testlanguage.cpp
        common/testpoint.cpp
        common/testrect.cpp
        common/testoutputformat.cpp
     )

     target_link_libraries(test_alphabets cuneiform-alphabets ${CPPUNIT_LIBRARIES})
     target_link_libraries(test_common cuneiform-common ${CPPUNIT_LIBRARIES})

     add_subdirectory(loaders)
     add_subdirectory(rreccom)

     ADD_TEST(Alphabets ${EXECUTABLE_OUTPUT_PATH}/test_alphabets)
     ADD_TEST(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)
     ADD_TEST(Loaders ${EXECUTABLE_OUTPUT_PATH}/test_loaders)
     ADD_TEST(RReccom ${EXECUTABLE_OUTPUT_PATH}/test_rreccom)

     message(STATUS "CppUnit library found: " ${CPPUNIT_LIBRARIES})
else()
     message(STATUS "CppUnit library not found. No tests will be build.")
endif()


