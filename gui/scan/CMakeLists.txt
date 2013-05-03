qt4_wrap_ui(CF_SCAN_UI_HEADERS scannerdialog.ui)
qt4_wrap_cpp(CF_SCAN_MOC_HEADERS
    kscandialog.h
    sanedialog.h
    scanner.h
    scannerdialog.h
    scanarea.h
)

set(QF_SCAN_LIB)

set(QF_SCAN_SRC
    kscandialog.cpp
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
    include_directories(${CMAKE_SOURCE_DIR}/contrib/libksane/libksane)
    list(APPEND QF_SCAN_LIB sanewidget ksane)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(quneiform_scan STATIC ${QF_SCAN_SRC})

target_link_libraries(quneiform_scan cuneiform-scan ${QF_SCAN_LIB})
