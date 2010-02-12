include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/h)
include_directories(../src/include)
include_directories(../src/hrstr)

find_package(CppUnit)
if(CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    message(STATUS "CppUnit include directory: " ${CPPUNIT_INCLUDE_DIR})
    message(STATUS "CppUnit library found: " ${CPPUNIT_LIBRARIES})
    
    add_subdirectory(alphabets)
    add_subdirectory(common)
    add_subdirectory(loaders)
    add_subdirectory(puma)
    add_subdirectory(rreccom)
    add_subdirectory(exporters)
else()
    message(STATUS "CppUnit library not found. No tests will be build.")
endif()


