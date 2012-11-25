# collect system information for saving it into config-verbose.h

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CF_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    set(CF_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    set(CF_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
elseif(CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    set(CF_CXX_FLAGS ${CMAKE_CXX_FLAGS_MINSIZEREL})
endif()

check_type_size("void*" CF_VOID_PTR_SIZE)
if(CF_VOID_PTR_SIZE EQUAL 8)
    set(CF_PLATFORM_BIT 64)
elseif(CF_VOID_PTR_SIZE EQUAL 4)
    set(CF_PLATFORM_BIT 32)
else()
    set(CF_PLATFORM_BIT -1)
endif()

if(CMAKE_CXX_COMPILER_ID)
    set(CF_COMPILER "${CMAKE_CXX_COMPILER_ID}")
else()
    if(MINGW)
        set(CF_COMPILER "MINGW")
    endif()

    if(MSVC)
        set(CF_COMPILER "MSVC")
    endif()

    if(MSYS)
        set(CF_COMPILER "MSYS")
    endif()

    if(CYGWIN)
        set(CF_COMPILER "CYGWIN")
    endif()
endif()

if(UNIX)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --version
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE CMAKE_CXX_COMPILER_RESULT
        OUTPUT_VARIABLE CMAKE_CXX_COMPILER_OUTPUT
        ERROR_VARIABLE CMAKE_CXX_COMPILER_ERROR
    )

    if(CMAKE_CXX_COMPILER_RESULT EQUAL 0)
        string(REPLACE "\n" " " CF_COMPILER_VERSION ${CMAKE_CXX_COMPILER_OUTPUT})
    endif()
endif()
