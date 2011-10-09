set(QUNEIFORM_VERSION ${CF_VERSION})

if(APPLE)
    project(Quneiform)
else()
    project(quneiform)
endif()

add_subdirectory(resources)

set(TRANSLATIONS_TS translations/ru.ts)

set(HEADERS 
    packet.h
    page.h
    pagerecognizer.h
    pagerecognitionqueue.h
    selection.h
    dialogs/aboutdialog.h
    dialogs/exportdialog.h
    dialogs/formatsettingsdialog.h
    dialogs/pagepropertiesdialog.h
    dialogs/packetopenprogressdialog.h
    dialogs/recognitionsettingsdialog.h
    dialogs/recognitionprogressdialog.h
    dialogs/settings.h
    widgets/colorbutton.h
    widgets/imageview.h
    widgets/imagewidget.h
    widgets/languagemenu.h
    widgets/languageselect.h
    widgets/mainwindow.h
    widgets/pageindicator.h
    widgets/recentmenu.h
    widgets/texteditor.h
    widgets/thumbnaillist.h
    widgets/thumbscene.h
    widgets/thumbnailwidget.h
    widgets/widgetbar.h
)

set(SOURCES
    exportsettings.cpp
    formatsettings.cpp
    imagecache.cpp
    language.cpp
    metatyperegistrator.cpp
    packet.cpp
    page.cpp
    pagearea.cpp
    pagelayout.cpp
    pagerecognizer.cpp
    pagerecognitionqueue.cpp
    recognitionsettings.cpp
    selection.cpp
    selectionshadow.cpp
    translationloader.cpp
    dialogs/aboutdialog.cpp
    dialogs/exportdialog.cpp
    dialogs/formatsettingsdialog.cpp
    dialogs/pagepropertiesdialog.cpp
    dialogs/packetopenprogressdialog.cpp
    dialogs/recognitionsettingsdialog.cpp
    dialogs/recognitionprogressdialog.cpp
    dialogs/settings.cpp
    export/cedpageexporter.cpp
    export/cedserializer.cpp
    export/cfexporter.cpp
    export/exporterfactory.cpp
    export/nativeexporter.cpp
    export/qtextdocumentexporter.cpp
    export/rectexporter.cpp
    widgets/colorbutton.cpp
    widgets/imageview.cpp
    widgets/imagewidget.cpp
    widgets/languagemenu.cpp
    widgets/languageselect.cpp
    widgets/mainwindow.cpp
    widgets/pageindicator.cpp
    widgets/recentmenu.cpp
    widgets/texteditor.cpp
    widgets/thumblayout.cpp
    widgets/thumbscene.cpp
    widgets/thumbnaillist.cpp
    widgets/thumbnailwidget.cpp
    widgets/widgetbar.cpp
)

set(FORMS 
    dialogs/aboutdialog.ui
    dialogs/exportdialog.ui
    dialogs/formatsettingsdialog.ui
    dialogs/pagepropertiesdialog.ui
    dialogs/recognitionsettingsdialog.ui
    dialogs/settings.ui
    widgets/mainwindow.ui
)

include_directories(
    ./widgets
    ../src
    ../src/include
    ../src/h
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RESOURCE_PATH resources)
set(RESOURCES     ${RESOURCE_PATH}/cuneiform.qrc)

set(QT_USE_QTMAIN TRUE)
find_package(Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

qt4_add_resources(QRC_SOURCES ${RESOURCES})
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_create_translation(TRANSLATIONS_QM ${HEADERS} ${SOURCES} ${UI_HEADERS} ${FORMS} ${TRANSLATIONS_TS})

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("Generated Files" FILES ${MOC_SOURCES})
source_group("Resource Files" FILES ${QRC_SOURCES} ${TRANSLATIONS_TS})

set(SRCS
    ${HEADERS}
    ${SOURCES}
    ${MOC_SOURCES}
    ${UI_HEADERS}
    export/iqfexporter.h
    quneiform_debug.h
)

set(EXTRA_RES)
list(APPEND EXTRA_RES ${TRANSLATIONS_QM})

if(MINGW)
    find_program(WINDRES
        NAMES windres.exe
        PATHS $ENV{PATH}
        NO_DEFAULT_PATH
    )
	
    if(WINDRES)
        message(STATUS "windres program found: ${WINDRES}")
	
        add_custom_command(
            OUTPUT
            win_quneiform_rc.o
            COMMAND
            windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
                -i${CMAKE_CURRENT_SOURCE_DIR}/resources/win_quneiform.rc
                -o win_quneiform_rc.o)
        list(APPEND EXTRA_RES win_quneiform_rc.o)
    endif()
elseif(MSVC)
	list(APPEND EXTRA_RES ${CMAKE_CURRENT_SOURCE_DIR}/resources/win_quneiform.rc)
endif()

add_subdirectory(spell)

if(APPLE)
    add_subdirectory(macosx)
endif()

add_library(quneiform_lib STATIC ${SRCS})

add_executable(${PROJECT_NAME} WIN32 main.cpp ${QRC_SOURCES} ${EXTRA_RES})

set(EXTRA_LIBS)
if(NOT MSVC)
    list(APPEND EXTRA_LIBS m)
endif()

# adding boost serialize
if(CF_SERIALIZE)
    list(APPEND EXTRA_LIBS ${Boost_SERIALIZATION_LIBRARY})
endif()

target_link_libraries (${PROJECT_NAME}
    quneiform_lib
    quneiform_spell
    ${LIBRARIES}
    ${QT_LIBRARIES}
    cuneiform
    ${EXTRA_LIBS})

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ${CMAKE_INSTALL_DESTINATION_ARGS}
)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(quneiform_lib UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()

install(FILES ${TRANSLATIONS_QM} DESTINATION ${RELATIVE_DATADIR}/locale)
