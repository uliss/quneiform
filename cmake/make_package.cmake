set(CPACK_SYSTEM_NAME i686)
set(CPACK_PACKAGE_VENDOR "Serj Poltavski")
set(CPACK_PACKAGE_CONTACT "serj.poltavski@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_NAME} - Optical Recognition Software")
set(CPACK_PACKAGE_VERSION "${CF_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR ${CF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CF_VERSION_MINOR})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")

if(WIN32)
    macro(cf_install_dll DLL_NAME src_path)
        unset(cf_file CACHE)
        find_file(cf_file "${DLL_NAME}.dll" PATHS ${src_path})
        if(cf_file)
            message(STATUS "Install: ${cf_file} found.")
            install(FILES
                        ${cf_file}
                        DESTINATION bin)
            
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${cf_file} ${CMAKE_BINARY_DIR})
        endif()
    endmacro()
endif()

if(APPLE)
    set(CPACK_GENERATOR "PackageMaker")
    set(CPACK_PACKAGE_NAME "Quneiform")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/doc/README.mac.txt")
    set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/doc/WELCOME.mac.txt")
    set(CPACK_SET_DESTDIR OFF)
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;TBZ2;TGZ")
    set(CPACK_STRIP_FILES "bin/cuneiform")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/readme.txt")
    # Debian
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serj Poltavski <serj.poltavski@gmail.com>")
    set(CF_DEB_DEPENDS "libc6 (>= 2.4), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0)")
    if(CF_SERIALIZE)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libboost-serialization1.38.0 (>= 1.38.0) | libboost-serialization1.40.0 (>= 1.40.0-1)")
    endif()
    if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libmagick++2 (>= 7:6.5.1)")
    endif()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${CF_DEB_DEPENDS})
elseif(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        cf_install_dll(QtCored4 ${QT_BINARY_DIR})
        cf_install_dll(QtGuid4 ${QT_BINARY_DIR})
    else()
        cf_install_dll(QtCore4 ${QT_BINARY_DIR})
        cf_install_dll(QtGui4 ${QT_BINARY_DIR})
    endif()
	
	if(MINGW)
	    find_path(CF_MINGW_PATH_BIN mingw32-make.exe PATHS
			"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin" 
			c:/MinGW/bin /MinGW/bin)
			
        cf_install_dll(libstdc++-6 ${CF_MINGW_PATH_BIN})
        cf_install_dll(libgcc_s_dw2-1 ${CF_MINGW_PATH_BIN})
        cf_install_dll(mingwm10 ${CF_MINGW_PATH_BIN})
        cf_install_dll(libiconv-2 ${ICONV_INCLUDE_DIR}/../bin)
    endif()
	
    if(MSVC)
        cf_install_dll(libiconv2 ${ICONV_INCLUDE_DIR}/../bin)
    endif()

    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "Cuneiform")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY
        "${CPACK_PACKAGE_NAME}")
    set(CPACK_PACKAGE_EXECUTABLES quneiform;Quneiform)
 
    # install icon
    set(CPACK_PACKAGE_ICON  "${CMAKE_SOURCE_DIR}/gui/resources\\\\cuneiform_install.bmp")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/gui/resources\\\\cuneiform.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/gui/resources\\\\cuneiform.ico")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME} ${CF_VERSION}")
	
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\quneiform.exe")
    set(CPACK_NSIS_CONTACT "serge.poltavski@gmail.com")
    set(CPACK_NSIS_HELP_LINK "https://code.launchpad.net/~serge-uliss/cuneiform-linux/gui")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://code.launchpad.net/~serge-uliss/cuneiform-linux/gui")
    set(CPACK_NSIS_MODIFY_PATH ON)

    # File types association:
    set(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}/cmake\\\\FileAssociation.nsh")

    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
          Push \\\"ATENDATA\\\"
          Push \\\"$INSTDIR\\\\share\\\\aten\\\"
          Call WriteEnvStr
    ")

    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
          \\\${registerExtension} \\\"Quneiform Packet\\\" \\\".qfp\\\" \\\"\\\$INSTDIR\\\\bin\\\\quneiform.exe\\\"
    ")

    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
          \\\${unregisterExtension} \\\".qfp\\\" \\\"Quneiform Packet\\\"
    ")
endif()

include(CPack)
include(InstallRequiredSystemLibraries)
