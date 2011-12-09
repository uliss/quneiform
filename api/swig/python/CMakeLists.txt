find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

execute_process(COMMAND
    ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_EXEC_PREFIX}')"
    OUTPUT_VARIABLE PYTHON_INSTDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Python install directory: ${PYTHON_INSTDIR}")

set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-c++")

SET_SOURCE_FILES_PROPERTIES(cuneiform-python.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(pyneiform python cuneiform-python.i)
SWIG_LINK_LIBRARIES(pyneiform ${PYTHON_LIBRARIES} cuneiform-c cuneiform)

set_target_properties(_pyneiform
    PROPERTIES
    INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
)

install(TARGETS _pyneiform
    EXPORT export_pyneiform
    LIBRARY DESTINATION "${PYTHON_INSTDIR}"
)

set(PERM_SCRIPTS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyneiform.py
        DESTINATION "${PYTHON_INSTDIR}"
)
