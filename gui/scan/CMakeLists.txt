qt4_wrap_ui(CF_SCAN_UI_HEADERS scannerdialog.ui)
qt4_wrap_cpp(CF_SCAN_MOC_HEADERS
    scanner.h
    scannerdialog.h
    scanarea.h)

set(QF_SCAN_SRC
    scanner.cpp
    scannerdialog.cpp
    scanneroption.cpp
    scanneroptionrange.cpp
    scanarea.cpp
    ${CF_SCAN_MOC_HEADERS}
    ${CF_SCAN_UI_HEADERS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(quneiform_scan STATIC ${QF_SCAN_SRC})

target_link_libraries(quneiform_scan cuneiform-scan)
