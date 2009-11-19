include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/h)
include_directories(../src/include)
include_directories(../src/hrk)

INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(CPPUNIT REQUIRED cppunit)

add_executable(test_alphabets
alphabets/test_alphabets.cpp
alphabets/testalphabet.cpp
alphabets/testalphabetfactory.cpp
)

add_executable(test_common
common/test_common.cpp
common/testpoint.cpp
common/testrect.cpp
)

add_executable(test_rfrmt
rfrmt/test_rfrmt.cpp
rfrmt/testcrtfchar.cpp
)

target_link_libraries(test_alphabets alphabets cppunit)
target_link_libraries(test_common cppunit)
target_link_libraries(test_rfrmt cppunit rfrmt puma alphabets)

ADD_TEST(Alphabets ${EXECUTABLE_OUTPUT_PATH}/test_alphabets)
ADD_TEST(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)
ADD_TEST(RFRMT ${EXECUTABLE_OUTPUT_PATH}/test_rfrmt)

