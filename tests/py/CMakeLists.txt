configure_file(${CMAKE_CURRENT_SOURCE_DIR}/all.py 
    ${CMAKE_BINARY_DIR}/py/all.py)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cf.py 
    ${CMAKE_BINARY_DIR}/py/cf.py)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lang_regression.py 
    ${CMAKE_BINARY_DIR}/py/lang_regression.py)
    
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lang_quick.py 
    ${CMAKE_BINARY_DIR}/py/lang_quick.py)
    
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/format_quick.py 
    ${CMAKE_BINARY_DIR}/py/format_quick.py)
    
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/align.py 
    ${CMAKE_BINARY_DIR}/py/align.py)
    
add_custom_target(test_all ${CMAKE_BINARY_DIR}/py/all.py)