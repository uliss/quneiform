file(GLOB_RECURSE THEME_ICONS *.png)

message(STATUS "Generating ${THEME} theme...")
set(THEME_QRC ${CMAKE_CURRENT_SOURCE_DIR}/theme_${THEME}.qrc)

function(GenerateTheme fname icons)
    string(LENGTH ${CMAKE_SOURCE_DIR}/ path_length)
    file(WRITE ${fname} "<!DOCTYPE RCC>\n<RCC version=\"1.0\">\n<qresource>\n")
    file(APPEND ${fname} "<file>index.theme</file>\n")
    foreach(icon ${icons})
        string(SUBSTRING ${icon} ${path_length} -1 local_icon)
        file(APPEND ${fname} "<file>${local_icon}</file>\n")
    endforeach()
    file(APPEND ${fname} "</qresource>\n</RCC>\n")
endfunction()

GenerateTheme(${THEME_QRC} "${THEME_ICONS}")

