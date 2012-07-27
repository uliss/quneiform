include(${QT_USE_FILE})

add_custom_target(generate_theme_oxygen
                   COMMAND ${CMAKE_COMMAND} -DTHEME=oxygen -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MINGW)
    set(OXYGEN_ROOT \\\\\\\\icons/oxygen)
else()
    set(OXYGEN_ROOT /icons/oxygen)
endif()

qt4_add_resources(THEME_OXYGEN_SRC theme_oxygen.qrc OPTIONS -root "${OXYGEN_ROOT}")
add_library(resource_theme_oxygen ${THEME_OXYGEN_SRC})
