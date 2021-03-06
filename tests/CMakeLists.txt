include_directories(../src)
include_directories(../src/hhh)
include_directories(../src/h)
include_directories(../src/include)
include_directories(../src/hrstr)

if(TESTS)
    find_package(CppUnit)
    if(CPPUNIT_FOUND)
        include_directories(${CPPUNIT_INCLUDE_DIR})
        message(STATUS "CppUnit include directory: " ${CPPUNIT_INCLUDE_DIR})
        message(STATUS "CppUnit library found: " ${CPPUNIT_LIBRARIES})
        message(STATUS "Tests: CppUnit tests will be build")
    
        add_subdirectory(alphabets)
        add_subdirectory(ced)
        add_subdirectory(cli)
        add_subdirectory(common)
        add_subdirectory(cimage)
        add_subdirectory(cpage)
        add_subdirectory(loaders)
        add_subdirectory(puma)
        add_subdirectory(rimage)
        add_subdirectory(rfrmt)
        add_subdirectory(rreccom)
        add_subdirectory(rselstr)
        add_subdirectory(rstr)
        add_subdirectory(exporters)
        add_subdirectory(scan)
        add_subdirectory(rdib)
        
        if(CF_USE_SHMEM)
            add_subdirectory(shmem)
        endif()

        if(CF_API)
            add_subdirectory(api)
        endif()

    else()
        message(STATUS "CppUnit library not found. No CppUnit tests will be build.")
        message(STATUS "Tests: Qt GUI tests will be build")
    endif()

    if(CF_GUI)
        add_subdirectory(quneiform)
    endif()
endif()

add_subdirectory(py)
