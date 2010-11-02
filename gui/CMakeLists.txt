set(PROJECT "quneiform")

set (HEADERS 
    document.h
    imagecache.h
 #   imagegraphicsview.h
    imageview.h
    imagewidget.h
    languageselect.h
    mainwindow.h
    page.h
    selection.h
    thumbnaillist.h
    thumbnailwidget.h
    widgetbar.h
)

set (SOURCES 
    document.cpp
    imagecache.cpp
#    imagegraphicsview.cpp
    imageview.cpp
    imagewidget.cpp
    languageselect.cpp
    mainwindow.cpp
    main.cpp
    page.cpp
    selection.cpp
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
 

#set (LIBRARIES#
#	cuneiform)
 
#set (LANGUAGES	rus	eng)
 
set (RESOURCE_PATH	resources)
set (RESOURCES ${RESOURCE_PATH}/cuneiform.qrc)
set (TRANSLATIONS_PATH	${RESOURCE_PATH}/translations)
 
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

qt4_add_resources (QRC_SOURCES ${RESOURCES})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
qt4_wrap_ui (UI_HEADERS ${FORMS} )

# translations
foreach (LANGUAGE ${LANGUAGES})
	set (TS ${TRANSLATIONS_PATH}/${LANGUAGE}.ts)
	set (QM ${TRANSLATIONS_PATH}/${LANGUAGE}.qm)
	set (TRANSLATIONS ${TRANSLATIONS} ${TS})
	set (TRANSLATIONS_BINARY ${TRANSLATIONS_BINARY} ${QM})
	add_custom_command (
		OUTPUT ${QM}
		COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS}
		MAIN_DEPENDENCY ${TS})
endforeach()

add_custom_target (
	translations 
	COMMAND ${QT_LUPDATE_EXECUTABLE} ${HEADERS} ${SOURCES} -ts ${TRANSLATIONS})
add_custom_command (
	TARGET translations
	COMMAND ${QT_LRELEASE_EXECUTABLE} ${TRANSLATIONS})
 
#foreach (LIBRARY ${LIBRARIES})
#	add_subdirectory (../${LIBRARY}/build bin/${LIBRARY})
#endforeach ()
 
source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})
source_group ("Resource Files" FILES ${QRC_SOURCES})
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${QRC_SOURCES} ${TRANSLATIONS})
 
target_link_libraries (${PROJECT} ${LIBRARIES} ${QT_LIBRARIES} m cuneiform)
