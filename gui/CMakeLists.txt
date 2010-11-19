set(PROJECT "quneiform")

set (TRANSLATIONS_QM ru.qm)
set (TRANSLATIONS_TS ru.ts)

set (HEADERS 
    document.h
    imagecache.h
    imageview.h
    imagewidget.h
    language_i18n.h
    languagemenu.h
    languageselect.h
    mainwindow.h
    page.h
    pagerecognizer.h
    pagerecognitionqueue.h
    selection.h
    selectionshadow.h
    thumbnaillist.h
    thumbnailwidget.h
    widgetbar.h
)

set (SOURCES 
    document.cpp
    imagecache.cpp
    imageview.cpp
    imagewidget.cpp
    language_i18n.cpp
    languagemenu.cpp
    languageselect.cpp
    mainwindow.cpp
    main.cpp
    page.cpp
    pagerecognizer.cpp
    pagerecognitionqueue.cpp
    selection.cpp
    selectionshadow.cpp
    thumbnaillist.cpp
    thumbnailwidget.cpp
    widgetbar.cpp
)

set (FORMS 
    mainwindow.ui)

include_directories(
    ../src
    ../src/include
    ../src/h
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set (RESOURCE_PATH resources)
set (RESOURCES     ${RESOURCE_PATH}/cuneiform.qrc)

find_package (Qt4 4.5 COMPONENTS QtCore QtGui QtOpenGl REQUIRED)
include (${QT_USE_FILE})

qt4_add_resources (QRC_SOURCES ${RESOURCES})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
qt4_wrap_ui (UI_HEADERS ${FORMS})
qt4_create_translation(${TRANSLATIONS_QM} ${HEADERS} ${SOURCES} ${UI_HEADERS} ${FORMS} ${TRANSLATIONS_TS})

 
source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})
source_group ("Resource Files" FILES ${QRC_SOURCES})
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${QRC_SOURCES} ${TRANSLATIONS_QM})
 
target_link_libraries (${PROJECT} ${LIBRARIES} ${QT_LIBRARIES} m cuneiform)
