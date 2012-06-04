qt4_wrap_ui(UI_HEADERS scannerdialog.ui)
qt4_wrap_cpp(MOC_HEADERS
    scanner.h
    scannerdialog.h
    scanarea.h)

set(QF_SCAN_SRC
    scanner.cpp
    scannerdialog.cpp
    scanneroption.cpp
    scanneroptionrange.cpp
    scanarea.cpp
    ${MOC_HEADERS}
    ${UI_HEADERS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(quneiform_scan STATIC ${QF_SCAN_SRC})

target_link_libraries(quneiform_scan cuneiform-scan)
