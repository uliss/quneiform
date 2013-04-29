set(QF_SCAN_LIB)
set(QF_SCAN_SRC
    abstractscannerdialog.cpp
    dummyscannerdialog.cpp
    externalappscannerdialog.cpp
)

qt4_wrap_cpp(QF_SCAN_MOC_HEADERS
    abstractscannerdialog.h
    dummyscannerdialog.h
    externalappscannerdialog.h
)

if(APPLE)
    qt4_wrap_cpp(MAC_MOC_HEADERS macscannerdialog.h)
    list(APPEND QF_SCAN_SRC macscannerdialog.cpp ${MAC_MOC_HEADERS})
    list(APPEND QF_SCAN_LIB quneiform_mac_scan)
endif()

list(APPEND QF_SCAN_SRC ${QF_SCAN_MOC_HEADERS})

if(CF_SANE_SCANNER_SUPPORT)
    qt4_wrap_cpp(CF_SCAN_MOC_HEADERS kscandialog.h sanescannerdialog.h)
    list(APPEND QF_SCAN_SRC kscandialog.cpp sanescannerdialog.cpp ${CF_SCAN_MOC_HEADERS})
    add_definitions(-DWITH_SANE)
    include_directories(${CMAKE_SOURCE_DIR}/contrib/libksane/libksane)
    list(APPEND QF_SCAN_LIB ksane)
endif()

if(CF_TWAIN_SCANNER_SUPPORT)
    qt4_wrap_cpp(CF_SCAN_MOC_HEADERS kscandialog.h twainscannerdialog.h)
    list(APPEND QF_SCAN_SRC kscandialog.cpp twainscannerdialog.cpp ${CF_SCAN_MOC_HEADERS})
    add_definitions(-DWITH_TWAIN)
    include_directories(${CMAKE_SOURCE_DIR}/contrib/libksane/libksane)
    list(APPEND QF_SCAN_LIB ksane)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(quneiform_scan STATIC ${QF_SCAN_SRC})

target_link_libraries(quneiform_scan quneiform_lib ${QF_SCAN_LIB})
