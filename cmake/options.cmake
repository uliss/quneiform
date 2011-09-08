if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(TESTS "Build tests" ON)
    option(OCRTK "Build OCRTk" ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option(TESTS "Build tests" OFF)
    option(OCRTK "Build OCRTk" OFF)
endif()

option(API "Build cuneiform language bindings" ON)
if(API)
    set(CF_API True)
endif()
