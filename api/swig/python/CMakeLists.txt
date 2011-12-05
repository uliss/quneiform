find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-c++")


SET_SOURCE_FILES_PROPERTIES(cuneiform.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(pyneiform python cuneiform.i)
SWIG_LINK_LIBRARIES(pyneiform ${PYTHON_LIBRARIES} cuneiform-c cuneiform)
