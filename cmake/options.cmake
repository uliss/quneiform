if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(TESTS "Build tests" ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option(TESTS "Build tests" OFF)
endif()
