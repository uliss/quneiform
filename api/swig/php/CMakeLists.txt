find_package(PHP5 REQUIRED)
include_directories(${PHP5_INCLUDE_DIR})
include_directories(${PHP5_ZEND_INCLUDE_DIR})
include_directories(${PHP5_TSRM_INCLUDE_DIR})
include_directories(${PHP5_MAIN_INCLUDE_DIR})

SET_SOURCE_FILES_PROPERTIES(cuneiform-php.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(php-cuneiform php cuneiform-php.i)
SWIG_LINK_LIBRARIES(php-cuneiform cuneiform-c cuneiform)
