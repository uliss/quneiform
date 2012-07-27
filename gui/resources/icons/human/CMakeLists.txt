include(${QT_USE_FILE})

add_custom_target(generate_theme_human
                   COMMAND ${CMAKE_COMMAND} -DTHEME=human -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MINGW)
    set(HUMAN_ROOT \\\\\\\\icons/human)
else()
    set(HUMAN_ROOT /icons/human)
endif()

qt4_add_resources(THEME_HUMAN_SRC theme_human.qrc OPTIONS -root ${HUMAN_ROOT})
add_library(resource_theme_human ${THEME_HUMAN_SRC})
