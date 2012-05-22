if(CMAKE_BUILD_TYPE STREQUAL Release)
    if(NOT MSVC)
        unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
        SET_COMPILER_FLAG_RELEASE(-O2)
    endif()

    message(STATUS "Release compiler flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()
