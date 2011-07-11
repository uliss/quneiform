find_package(PythonInterp)


macro(install_test_py file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}
        ${CMAKE_BINARY_DIR}/py/${file})
endmacro()

if(PYTHONINTERP_FOUND)
    if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        set(CF_DIFFOPTS "-b")
    else()
        set(CF_DIFFOPTS "-bB")
    endif()

    install_test_py(cf.py)
    install_test_py(columns.py)
    install_test_py(align.py)
    install_test_py(all.py)
    install_test_py(font_styles.py)
    install_test_py(format_odf.py)
    install_test_py(format_quick.py)
    install_test_py(hyphens.py)
    install_test_py(image_formats.py)
    install_test_py(image_export.py)
    install_test_py(indent.py)
    install_test_py(lang_regression.py)
    install_test_py(lang_quick.py)
    
    add_custom_target(test_all ${CMAKE_BINARY_DIR}/py/all.py)
endif()
