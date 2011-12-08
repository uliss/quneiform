find_package(JNI REQUIRED)
find_package(Java COMPONENTS Development REQUIRED)
include_directories(${JNI_INCLUDE_DIRS} ${JAVA_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS})
set(SWIG_JAVA_MODULE Juneiform)

set_source_files_properties(cuneiform-java.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(${SWIG_JAVA_MODULE} java cuneiform-java.i)
SWIG_LINK_LIBRARIES(${SWIG_JAVA_MODULE} ${PERL_LIBRARY} cuneiform-c cuneiform)

