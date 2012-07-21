add_custom_target(generate_theme_faenza
                   COMMAND ${CMAKE_COMMAND} -DTHEME=faenza -P ${CMAKE_SOURCE_DIR}/cmake/generate_icontheme.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_add_resources(THEME_FAENZA_SRC theme_faenza.qrc OPTIONS -root /icons/faenza)
add_library(resource_theme_faenza ${THEME_FAENZA_SRC})
