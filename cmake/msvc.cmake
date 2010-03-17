# Stop MSVC complaints about POSIX functions.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT MSVC60)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    endif()
endif()