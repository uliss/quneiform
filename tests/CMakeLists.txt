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
    add_subdirectory(ced)
    add_subdirectory(common)
    add_subdirectory(loaders)
    add_subdirectory(puma)
    add_subdirectory(rfrmt)
    add_subdirectory(rreccom)
    add_subdirectory(exporters)
else()
    message(STATUS "CppUnit library not found. No tests will be build.")
endif()

if(CF_OCRTK)
    add_subdirectory(ocrtk)
endif()

configure_file(${CMAKE_SOURCE_DIR}/tests/lang_regression.py 
    ${CMAKE_BINARY_DIR}/lang_regression.py)
    
configure_file(${CMAKE_SOURCE_DIR}/tests/quick_lang_test.py 
    ${CMAKE_BINARY_DIR}/quick_lang_test.py)
    
configure_file(${CMAKE_SOURCE_DIR}/tests/quick_format_test.py 
    ${CMAKE_BINARY_DIR}/quick_format_test.py)
    
configure_file(${CMAKE_SOURCE_DIR}/tests/test_align.py 
    ${CMAKE_BINARY_DIR}/test_align.py)



