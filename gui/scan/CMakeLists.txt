qt4_wrap_ui(CF_SCAN_UI_HEADERS scannerdialog.ui)
qt4_wrap_cpp(CF_SCAN_MOC_HEADERS
    sanedialog.h
    scanner.h
    scannerdialog.h
    scanarea.h
)

set(QF_SCAN_LIB)

set(QF_SCAN_SRC
    sanedialog.cpp
    scanner.cpp
    scannerdialog.cpp
    scanneroption.cpp
    scanneroptionrange.cpp
    scanarea.cpp
    ${CF_SCAN_MOC_HEADERS}
    ${CF_SCAN_UI_HEADERS})

if(CF_SANE_SCANNER_SUPPORT)
    add_definitions(-DWITH_SANE)
    include_directories(${CMAKE_SOURCE_DIR}/contrib/libsanewidget/src)
    list(APPEND QF_SCAN_LIB sanewidget)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(quneiform_scan STATIC ${QF_SCAN_SRC})

target_link_libraries(quneiform_scan cuneiform-scan ${QF_SCAN_LIB})
