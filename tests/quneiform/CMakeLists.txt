add_definitions(-DCF_IMAGE_DIR="${CMAKE_SOURCE_DIR}/images/lang.diftest")
find_package(Qt4 4.5 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTTEST_INCLUDE_DIR})
include_directories("../..")

set(CF_GUI_TESTS
    imagecache
    page
    packet
    recognitionsettingsdialog
    pageindicator
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test_gui.py"
        "${CMAKE_CURRENT_BINARY_DIR}/test_gui.py")

function(add_gui_tests)
    foreach(t ${CF_GUI_TESTS})
        unset(cf_test_moc)
        qt4_wrap_cpp(cf_test_moc "test${t}.h")
        add_executable("test_${t}" "test${t}.h" "test${t}.cpp" ${cf_test_moc})
        target_link_libraries("test_${t}" quneiform_lib ${QT_LIBRARIES} ${QT_QTTEST_LIBRARIES})
        add_test("Quneiform:test_${t}" "${EXECUTABLE_OUTPUT_PATH}/test_${t}")
    endforeach()
endfunction()

add_gui_tests()
add_custom_target(test_gui "${CMAKE_CURRENT_BINARY_DIR}/test_gui.py")
