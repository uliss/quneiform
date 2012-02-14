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
    find_package(Perl)
    install_test_script(test_perl_api.pl)
    add_test(PerlAPI ${EXECUTABLE_OUTPUT_PATH}/test_perl_api.pl)
endif()

if(WITH_JAVA)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/TestJavaApi.class
        COMMAND ${Java_JAVAC_EXECUTABLE}
        ARGS "-classpath" "${CMAKE_BINARY_DIR}/api/swig/java"
        ${CMAKE_CURRENT_BINARY_DIR}/TestJavaApi.java
    )

    add_custom_target(TestJavaApi
        ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/TestJavaApi.class)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestJavaApi.java
        ${CMAKE_CURRENT_BINARY_DIR}/TestJavaApi.java)

    add_test(JavaAPI
        java
            "-Djava.library.path=${CMAKE_BINARY_DIR}/api/swig/java"
            "-classpath" "${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}/api/swig/java"
            "TestJavaApi")
endif()

if(WITH_PHP)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_php_api.php
        ${CMAKE_CURRENT_BINARY_DIR}/test_php_api.php)
    add_test(PhpAPI ${PHP5_EXECUTABLE}
                -d enable_dl=On
                -d extension=${CMAKE_BINARY_DIR}/api/swig/php/libphp-cuneiform.so
                ${CMAKE_CURRENT_BINARY_DIR}/test_php_api.php)
endif()
