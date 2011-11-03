set(CF_VERSION "0.2")

if(EXISTS "@CMAKE_SOURCE_DIR@/version")
    file(READ "@CMAKE_SOURCE_DIR@/version" CF_BUILD_NUMBER)
else()
    message(STATUS "Can't find build number, using build version")
    set(CF_BUILD_NUMBER ${CF_VERSION})
    file(WRITE "@CMAKE_SOURCE_DIR@/version" "${CF_BUILD_NUMBER}")
endif()

string(REGEX MATCHALL "[0-9]+" CF_VERSION_LIST ${CF_BUILD_NUMBER})

list(GET CF_VERSION_LIST 0 CF_VERSION_MAJOR)
list(GET CF_VERSION_LIST 1 CF_VERSION_MINOR)
set(CF_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}")

message(STATUS "Build version is: ${CF_VERSION}")
