if(CMAKE_BUILD_TYPE STREQUAL Release)
    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
        SET_COMPILER_DEBUG_FLAG(-O2)
    endif()

    message(STATUS "Compiler flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()