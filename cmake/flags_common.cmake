add_definitions("-D_USE_RVERLINE_")

if(MINGW)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mwindows")
endif()