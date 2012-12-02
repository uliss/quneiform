set(CPACK_SYSTEM_NAME i686)
set(CPACK_PACKAGE_VENDOR "Serge Poltavski")
set(CPACK_PACKAGE_CONTACT "serge.poltavski@gmail.com")
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
                        DESTINATION bin
                        COMPONENT Applications
                    )
            
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${cf_file} ${CMAKE_BINARY_DIR})
        endif()
    endmacro()
endif()
 
if(APPLE)
    add_custom_target(dmg COMMAND "${CMAKE_SOURCE_DIR}/cmake/makeapp.sh"
                                  "${CMAKE_BINARY_DIR}" # dest dir
                                  "${CMAKE_SOURCE_DIR}" # source dir
                                  "${CMAKE_BINARY_DIR}" # build dir
                          DEPENDS  Quneiform
)
endif()

if(APPLE)
    set(CPACK_GENERATOR "Bundle")
    set(CPACK_BUNDLE_NAME "Quneiform")

    set(MACOSX_BUNDLE_BUNDLE_NAME ${CPACK_BUNDLE_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${CF_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "(c) 2010-2012 Serge Poltavski")
    set(MACOSX_BUNDLE_EXECUTABLE_NAME "Quneiform")
    set(MACOSX_BUNDLE_ICON_FILE "Quneiform.icns")
    set(MACOSX_BUNDLE_INFO_STRING "Optical recognition software")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.uliss.quneiform")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${CPACK_BUNDLE_NAME} ${CF_BUILD_NUMBER}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${CF_VERSION})

    configure_file(${CMAKE_SOURCE_DIR}/cmake/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist)

    set(CPACK_PACKAGE_NAME ${CPACK_BUNDLE_NAME})
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/gui/resources/cuneiform.icns")

    set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/gui/resources/cuneiform.icns")
    set(CPACK_BUNDLE_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    set(CPACK_BUNDLE_STARTUP_COMMAND ${CMAKE_SOURCE_DIR}/cmake/MacOSX.sh)

    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/doc/README.mac.txt")
    set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/doc/WELCOME.mac.txt")

    set(CPACK_SET_DESTDIR OFF)

elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;TBZ2;TGZ")
    set(CPACK_STRIP_FILES "bin/cuneiform")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
    # Debian
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serge Poltavski <serge.poltavski@gmail.com>")
    set(CF_DEB_DEPENDS "libc6 (>= 2.4), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0)")
    if(CF_SERIALIZE)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS},  libboost-serialization1.38.0 (>= 1.38.0) | libboost-serialization1.42.0 (>= 1.42.0) | libboost-serialization1.40.0 (>= 1.40.0-1) | libboost-serialization1.46.1 (>= 1.46.1)")
    endif()
    if(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libmagick++2 (>= 7:6.5.1) | libmagick++3 (>= 8:6.6.0.4)")
    endif()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${CF_DEB_DEPENDS})
elseif(WIN32)    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        cf_install_dll(QtCored4 ${QT_BINARY_DIR})
        cf_install_dll(QtGuid4 ${QT_BINARY_DIR})
    else()
        cf_install_dll(QtCore4 ${QT_BINARY_DIR})
        cf_install_dll(QtGui4 ${QT_BINARY_DIR})
        cf_install_dll(zlib1 ${ZLIB_INCLUDE_DIRS}/../bin)
    endif()
    
    # install FreeImage
    if(CF_USE_IMAGE_LIBRARY_FREEIMAGE)
        cf_install_dll(FreeImage ${FREEIMAGE_INCLUDE_PATH})
    endif()
    
    # install boost
    if(Boost_SERIALIZATION_FOUND AND CF_EXPORT_LIBTYPE STREQUAL "SHARED")
        set(boost_dll "")
        
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(boost_lib_path ${Boost_SERIALIZATION_LIBRARY_DEBUG})
        else()
            set(boost_lib_path ${Boost_SERIALIZATION_LIBRARY_RELEASE})
        endif()
        
        get_filename_component(boost_dll "${boost_lib_path}" NAME_WE)
        #message(STATUS ${Boost_LIBRARY_DIRS})
        cf_install_dll(${boost_dll} ${Boost_LIBRARY_DIRS})
    endif()
	
	if(MINGW)
	    find_path(CF_MINGW_PATH_BIN mingw32-make.exe PATHS
			"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin" 
			c:/MinGW/bin /MinGW/bin)
			
        cf_install_dll(libstdc++-6 ${CF_MINGW_PATH_BIN})
        cf_install_dll(libgcc_s_dw2-1 ${CF_MINGW_PATH_BIN})
        cf_install_dll(mingwm10 ${CF_MINGW_PATH_BIN})
        cf_install_dll(libiconv-2 ${ICONV_INCLUDE_DIR}/../bin)
        cf_install_dll(libiconv2 ${ICONV_INCLUDE_DIR}/../bin)
    endif()
	
    if(MSVC)
        cf_install_dll(libiconv2 ${ICONV_INCLUDE_DIR}/../bin)
    endif()

    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "Quneiform")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY
        "${CPACK_PACKAGE_NAME}")
    set(CPACK_PACKAGE_EXECUTABLES quneiform;Quneiform)
    
    # components setup
    set(CPACK_NSIS_COMPONENT_INSTALL ON)
    #set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
    set(CPACK_COMPONENTS_ALL Applications Libraries Headers)
    
    # app
    set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Quneiform OCR application")
    set(CPACK_COMPONENT_APPLICATIONS_DEPENDS Libraries)
    set(CPACK_COMPONENT_APPLICATIONS_REQUIRED ON)
    # libs
    set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
    set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Libraries used to build programs with cuneiform")
    set(CPACK_COMPONENT_LIBRARIES_REQUIRED ON)
    # headers
    set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")
    set(CPACK_COMPONENT_HEADERS_DESCRIPTION "C++ header files for use with cuneiform")
    set(CPACK_COMPONENT_HEADERS_DEPENDS Libraries)
    set(CPACK_COMPONENT_HEADERS_DISABLED ON) 
 
    # install icon
    set(CPACK_PACKAGE_ICON  "${CMAKE_SOURCE_DIR}/gui/resources\\\\cuneiform_install.bmp")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/gui/resources\\\\cuneiform.ico")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME} ${CF_VERSION}")
	
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\quneiform.exe")
    set(CPACK_NSIS_CONTACT "serge.poltavski@gmail.com")
    set(CPACK_NSIS_HELP_LINK "https://github.com/uliss/quneiform")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/uliss/quneiform")
    set(CPACK_NSIS_MODIFY_PATH ON)

    set(CPACK_NSIS_INCLUDE "!include ${CMAKE_SOURCE_DIR}/cmake\\\\FileAssociation.nsh")
endif()

include(CPack)
include(InstallRequiredSystemLibraries)
