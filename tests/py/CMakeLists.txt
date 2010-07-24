macro(install_test_py file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}
        ${CMAKE_BINARY_DIR}/py/${file})
endmacro()

install_test_py(all.py)
install_test_py(cf.py)
install_test_py(lang_regression.py)
install_test_py(lang_quick.py)
install_test_py(format_quick.py)
install_test_py(align.py)
install_test_py(indent.py)
install_test_py(image_formats.py)
install_test_py(image_export.py)
    
add_custom_target(test_all ${CMAKE_BINARY_DIR}/py/all.py)