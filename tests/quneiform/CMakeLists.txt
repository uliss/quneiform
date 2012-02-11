add_definitions(-DCF_IMAGE_DIR="${CMAKE_SOURCE_DIR}/images/quneiform")
find_package(Qt4 4.5 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTTEST_INCLUDE_DIR})
include_directories("../..")
include_directories("../../gui")

set(CF_GUI_TESTS
    cedpageexporter
    exportsettings
    formatsettings
    imagecache
    imageview
    language
    languageselect
    page
    packet
    pageindicator
    pagelayout
    pagerecognitionqueue
    pagerecognizer
    recentmenu
    recognitionsettingsdialog
    recognitionprogressdialog
    selection
    spellcheckerfactory
    texteditor
    qtextdocumentexporter
)

if(CF_SERIALIZE)
    list(APPEND CF_GUI_TESTS cedserializer)
endif()

if(APPLE)
    list(APPEND CF_GUI_TESTS macstring)
    list(APPEND CF_GUI_TESTS macspellchecker)
endif()

find_package(ASPELL)
if(ASPELL_FOUND)
    add_definitions(-DHAVE_ASPELL)
    list(APPEND CF_GUI_TESTS aspellchecker)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test_gui.py"
        "${CMAKE_CURRENT_BINARY_DIR}/test_gui.py")

set(TEST_LIBS quneiform_spell quneiform_lib quneiform_spell cuneiform ${QT_LIBRARIES} ${QT_QTTEST_LIBRARIES})

if(APPLE)
    find_library(APPLE_CORE_FOUNDATION CoreFoundation "/")
    list(APPEND TEST_LIBS ${APPLE_CORE_FOUNDATION})
    find_library(APPLE_APPKIT AppKit "/")
    list(APPEND TEST_LIBS ${APPLE_APPKIT})
endif()

if(CF_SERIALIZE)
    list(APPEND TEST_LIBS ${Boost_SERIALIZATION_LIBRARY})
endif()

function(add_gui_tests)
    foreach(t ${CF_GUI_TESTS})
        unset(cf_test_moc)
        qt4_wrap_cpp(cf_test_moc "test${t}.h")
        add_executable("test_${t}" "test${t}.h" "test${t}.cpp" ${cf_test_moc})
        target_link_libraries("test_${t}" ${TEST_LIBS})
        add_test("GUI:test_${t}" "${EXECUTABLE_OUTPUT_PATH}/test_${t}")
    endforeach()
endfunction()

add_gui_tests()
add_custom_target(test_gui "${CMAKE_CURRENT_BINARY_DIR}/test_gui.py")
