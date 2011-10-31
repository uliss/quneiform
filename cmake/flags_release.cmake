if(CMAKE_BUILD_TYPE STREQUAL Release)
    unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
    SET_COMPILER_FLAG_RELEASE(-O2)

    message(STATUS "Release compiler flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()
