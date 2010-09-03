macro(install_py file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}
        ${CMAKE_BINARY_DIR}/py/${file})
endmacro()

install_py(binary_diff.py)
install_py(cuneiform_md5.py)

add_custom_target(binary_diff ${CMAKE_BINARY_DIR}/py/binary_diff.py)