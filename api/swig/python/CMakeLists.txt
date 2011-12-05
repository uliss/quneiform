find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

SWIG_ADD_MODULE(pyneiform python cuneiform.i)
SWIG_LINK_LIBRARIES(pyneiform ${PYTHON_LIBRARIES} cuneiform-c cuneiform)
