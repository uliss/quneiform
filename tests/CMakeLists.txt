include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/include)
include_directories(../src/hrk)

add_executable(test_alphabets
alphabets/test_alphabets.cpp
alphabets/testalphabet.cpp
alphabets/testalphabetfactory.cpp
)



target_link_libraries(test_alphabets alphabets cppunit)

ADD_TEST(test_alphabets ${EXECUTABLE_OUTPUT_PATH}/test_alphabets)

