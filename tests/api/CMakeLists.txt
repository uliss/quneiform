include_directories(../../api)
add_definitions(-DTEST_IMG_PATH="${CMAKE_SOURCE_DIR}/images/quneiform")

add_executable(test_c_api
    test_c_api.cpp
    testcapi.cpp
)

target_link_libraries(test_c_api cuneiform-c cuneiform ${CPPUNIT_LIBRARIES})

add_test(CApi ${EXECUTABLE_OUTPUT_PATH}/test_c_api)

find_package(PythonInterp)

macro(install_test_script file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}
        ${EXECUTABLE_OUTPUT_PATH}/${file})
endmacro()

if(WITH_PYTHON)
    install_test_script(test_python_api.py)
    add_test(PythonAPI ${EXECUTABLE_OUTPUT_PATH}/test_python_api.py)
endif()

if(WITH_PERL)
    install_test_script(test_perl_api.pl)
    add_test(PerlAPI ${EXECUTABLE_OUTPUT_PATH}/test_perl_api.pl)
endif()
