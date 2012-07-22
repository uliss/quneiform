add_custom_target(generate_theme_human
                   COMMAND ${CMAKE_COMMAND} -DTHEME=human -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_add_resources(THEME_HUMAN_SRC theme_human.qrc OPTIONS -root /icons/human)
add_library(resource_theme_human ${THEME_HUMAN_SRC})
