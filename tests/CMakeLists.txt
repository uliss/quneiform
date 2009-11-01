include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/include)
include_directories(../src/hrk)

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

target_link_libraries(test_alphabets alphabets cppunit)
target_link_libraries(test_common cppunit)

ADD_TEST(Alphabets ${EXECUTABLE_OUTPUT_PATH}/test_alphabets)
ADD_TEST(Common ${EXECUTABLE_OUTPUT_PATH}/test_common)

