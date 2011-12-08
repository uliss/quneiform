find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS} ${JAVA_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-c++")
set(SWIG_JAVA_MODULE Juneiform)

SET_SOURCE_FILES_PROPERTIES(cuneiform-java.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(${SWIG_JAVA_MODULE} java cuneiform-java.i)
SWIG_LINK_LIBRARIES(${SWIG_JAVA_MODULE} ${PERL_LIBRARY} cuneiform-c cuneiform)

#set_target_properties(_pyneiform
#    PROPERTIES
#    INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
#)

#install(TARGETS _pyneiform
#    EXPORT export_pyneiform
#    LIBRARY DESTINATION "${PYTHON_INSTDIR}"
#)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyneiform.py
#        DESTINATION "${PYTHON_INSTDIR}"
#)
