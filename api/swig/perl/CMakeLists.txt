find_package(PerlLibs REQUIRED)
include_directories(${PERL_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-c++")
set(SWIG_PERL_MODULE Cuneiform)

SET_SOURCE_FILES_PROPERTIES(cuneiform-perl.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(${SWIG_PERL_MODULE} perl cuneiform-perl.i)
SWIG_LINK_LIBRARIES(${SWIG_PERL_MODULE} ${PERL_LIBRARY} cuneiform-c cuneiform)

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
