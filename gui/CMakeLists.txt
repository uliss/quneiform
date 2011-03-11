set(PROJECT "quneiform")

add_subdirectory(resources)

set(TRANSLATIONS_TS ru.ts)

set(HEADERS 
    aboutdialog.h
    imageview.h
    imagewidget.h
    languagemenu.h
    languageselect.h
    mainwindow.h
    packet.h
    page.h
    pageindicator.h
    pagerecognizer.h
    pagerecognitionqueue.h
    recentmenu.h
    recognitionsettingsdialog.h
    recognitionprogressdialog.h
    selection.h
    settings.h
    thumbnaillist.h
    thumbnailwidget.h
    widgetbar.h
)

set(SOURCES 
    aboutdialog.cpp
    imagecache.cpp
    imageview.cpp
    imagewidget.cpp
    language_i18n.cpp
    languagemenu.cpp
    languageselect.cpp
    mainwindow.cpp
    packet.cpp
    page.cpp
    pageindicator.cpp
    pagelayout.cpp
    pagerecognizer.cpp
    pagerecognitionqueue.cpp
    recentmenu.cpp
    recognitionsettings.cpp
    recognitionsettingsdialog.cpp
    recognitionprogressdialog.cpp
    rectexporter.cpp
    selection.cpp
    selectionshadow.cpp
    settings.cpp
    thumbnaillist.cpp
    thumbnailwidget.cpp
    translationloader.cpp
    widgetbar.cpp
)

set(FORMS 
    aboutdialog.ui
    mainwindow.ui
    recognitionsettingsdialog.ui
    settings.ui
)

include_directories(
    ../src
    ../src/include
    ../src/h
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RESOURCE_PATH resources)
set(RESOURCES     ${RESOURCE_PATH}/cuneiform.qrc)

find_package(Qt4 4.5 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

qt4_add_resources(QRC_SOURCES ${RESOURCES})
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_create_translation(TRANSLATIONS_QM ${HEADERS} ${SOURCES} ${UI_HEADERS} ${FORMS} ${TRANSLATIONS_TS})

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("Generated Files" FILES ${MOC_SOURCES})
source_group("Resource Files" FILES ${QRC_SOURCES})

set(SRCS
    ${HEADERS}
    ${SOURCES}
    ${MOC_SOURCES}
    ${UI_HEADERS}
    ${TRANSLATIONS_QM}
)

if(MINGW)
	find_program(WINDRES
		NAMES windres.exe
		PATHS $ENV{PATH}
		NO_DEFAULT_PATH
	) 
	
	if(WINDRES) 
		message(STATUS "windres programm found: ${WINDRES}")
	
		add_custom_command(
		OUTPUT 
			win_quneiform_rc.o
		COMMAND 
			windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} 
				-i${CMAKE_CURRENT_SOURCE_DIR}/resources/win_quneiform.rc 
				-o win_quneiform_rc.o)
		set(SRCS ${SRCS} win_quneiform_rc.o)
	endif()
endif()

if(APPLE)
    set(CMAKE_BUNDLE_NAME "Quneiform")
    set(MACOSX_BUNDLE_BUNDLE_NAME "Quneiform")
    set(MACOSX_BUNDLE_COPYRIGHT "(c) 2010-2011 Serge Poltavski" )

    # setting bundle icon
    set(ICON cuneiform.icns)
    set(ICON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/resources/${ICON}")
    set(SRCS ${SRCS} ${ICON_SRC})
    set(MACOSX_BUNDLE_ICON_FILE ${ICON})

    # where to place resources
    set_source_files_properties(${ICON_SRC} ${TRANSLATIONS_QM}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    # bundle l10n
    add_custom_target(osx_l10n_dirs
        COMMAND mkdir -p "${CMAKE_BINARY_DIR}/Quneiform.app/Contents/Resources/ru.lproj"
        COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/resources/ru.locversion.plist"
                    "${CMAKE_BINARY_DIR}/Quneiform.app/Contents/Resources/ru.lproj/locversion.plist"
    )
endif()

add_library(quneiform_lib STATIC ${SRCS})
add_executable (${PROJECT} WIN32 MACOSX_BUNDLE main.cpp ${QRC_SOURCES})
 
if(APPLE)
	add_dependencies(${PROJECT} osx_l10n_dirs)
    find_library(EXTRA_LIBS CoreFoundation "/")
else()
    set(EXTRA_LIBS "")
endif()

if(NOT MSVC)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
else()
    set(${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
endif()

target_link_libraries (${PROJECT} quneiform_lib ${LIBRARIES} ${QT_LIBRARIES} cuneiform ${EXTRA_LIBS})

if(APPLE)
    set_target_properties(${PROJECT}
        PROPERTIES
        OUTPUT_NAME ${CMAKE_BUNDLE_NAME})

    set(CMAKE_INSTALL_DESTINATION_ARGS
        BUNDLE DESTINATION "/Applications")
endif()

install(
    TARGETS ${PROJECT}
    RUNTIME DESTINATION bin
    ${CMAKE_INSTALL_DESTINATION_ARGS}
)

install(FILES ${TRANSLATIONS_QM} DESTINATION ${RELATIVE_DATADIR}/locale)
