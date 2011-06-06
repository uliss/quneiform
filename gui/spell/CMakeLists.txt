set(SOURCES
    ispellchecker.cpp
    spellcheckhighlighter.cpp
    spellcheckerfactory.cpp
)

if(APPLE) 
    list(APPEND SOURCES macspellchecker.cpp)
    list(APPEND SOURCES macspellchecker_private.mm) 
else()
    list(APPEND SOURCES aspellchecker.cpp)
endif()

source_group("Source Files" FILES ${SOURCES})

add_library(quneiform_spell STATIC ${SOURCES})

if(APPLE)
    target_link_libraries(quneiform_spell quneiform_mac)
endif()



