add_custom_target(generate_theme_mac
                   COMMAND ${CMAKE_COMMAND} -DTHEME=mac -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_add_resources(THEME_MAC_SRC theme_mac.qrc OPTIONS -root /icons/mac)
add_library(resource_theme_mac ${THEME_MAC_SRC})
