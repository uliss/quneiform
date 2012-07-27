include(${QT_USE_FILE})

add_custom_target(generate_theme_snowish
                   COMMAND ${CMAKE_COMMAND} -DTHEME=snowish -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MINGW)
    set(SNOWISH_ROOT \\\\\\\\icons/snowish)
else()
    set(SNOWISH_ROOT /icons/snowish)
endif()

qt4_add_resources(THEME_SNOWISH_SRC theme_snowish.qrc OPTIONS -root ${SNOWISH_ROOT})
add_library(resource_theme_snowish ${THEME_SNOWISH_SRC})
