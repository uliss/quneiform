add_custom_target(generate_theme_gnome
                   COMMAND ${CMAKE_COMMAND} -DTHEME=gnome -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_add_resources(THEME_GNOME_SRC theme_gnome.qrc OPTIONS -root /icons/gnome)
add_library(resource_theme_gnome ${THEME_GNOME_SRC})
