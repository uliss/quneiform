set(CPACK_SYSTEM_NAME i686)
set(CPACK_PACKAGE_VENDOR "Serj Poltavski")
set(CPACK_PACKAGE_CONTACT "serj.poltavski@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_NAME} - Optical Recognition Software")
set(CPACK_PACKAGE_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR ${CF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CF_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CF_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")

if(APPLE)
    set(CPACK_GENERATOR "PackageMaker")
    set(CPACK_PACKAGE_NAME "Quneiform")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/doc/README.mac.txt")
    set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/doc/WELCOME.mac.txt")
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;TBZ2;TGZ")
    set(CPACK_STRIP_FILES "bin/cuneiform")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/readme.txt")
    # Debian
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serj Poltavski <serj.poltavski@gmail.com>")
    set(CF_DEB_DEPENDS "libc6 (>= 2.4), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0)")
    if(CF_USE_ZIP)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libzip1 (>= 0.8)")
    endif()
    if(CF_SERIALIZE)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libboost-serialization1.38.0 (>= 1.38.0) | libboost-serialization1.40.0 (>= 1.40.0-1)")
    endif()
    if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libmagick++2 (>= 7:6.5.1)")
    endif()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${CF_DEB_DEPENDS})
elseif(WIN32)	
	install(FILES
		"${QT_LIBRARY_DIR}/../bin/QtCore${QT_VERSION_MAJOR}.dll"
		"${QT_LIBRARY_DIR}/../bin/QtGui${QT_VERSION_MAJOR}.dll"
		
		DESTINATION bin
	)

	set(CPACK_GENERATOR "NSIS")	
	set(CPACK_PACKAGE_NAME "Quneiform")
	set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY
		"${CPACK_PACKAGE_NAME}")
	
	set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/gui/resources/cuneiform.ico")
	#set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/gui/resources/uninstall.ico")
	
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\quneiform.exe")
	set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${PROJECT_NAME}")
	set(CPACK_NSIS_CONTACT "serge.poltavski@gmail.com")
	set(CPACK_NSIS_MODIFY_PATH ON)
endif()

include(CPack)
include(InstallRequiredSystemLibraries)
