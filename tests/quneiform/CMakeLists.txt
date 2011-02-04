find_package(Qt4 4.5 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTTEST_INCLUDE_DIR})
include_directories("../..")

set(CF_GUI_TESTS
    page
    imagecache
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test_gui.py"
        "${CMAKE_BINARY_DIR}/quneiform/test_gui.py")

foreach(t ${CF_GUI_TESTS})
    qt4_wrap_cpp(cf_moc "test${t}.h")
    add_executable("test_${t}" "test${t}.cpp" "test${t}.h" ${cf_moc})
    target_link_libraries("test_${t}" quneiform_lib ${QT_LIBRARIES} ${QT_QTTEST_LIBRARIES})
    add_test("Quneiform:test_${t}" "${EXECUTABLE_OUTPUT_PATH}/test_${t}")
endforeach()

add_custom_target(test_gui "${CMAKE_BINARY_DIR}/quneiform/test_gui.py")
