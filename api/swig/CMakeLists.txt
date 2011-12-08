find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../c)
set(CMAKE_SWIG_FLAGS "-ignoremissing" "-Wall" "-includeall")

if(WITH_PYTHON)
    add_subdirectory(python)
endif()

if(WITH_PERL)
    add_subdirectory(perl)
endif()

if(WITH_JAVA)
    add_subdirectory(java)
endif()
