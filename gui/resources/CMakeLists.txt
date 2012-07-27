if(UNIX AND NOT APPLE)
    # install a desktop file so Quneiform appears in the application start menu
    # with an icon
    INSTALL(FILES quneiform.desktop DESTINATION share/applications )
    INSTALL(FILES cuneiform_logo.png DESTINATION share/pixmaps )
    INSTALL(FILES quneiform.xml DESTINATION share/mime/packages )
endif()

add_subdirectory(icons)
