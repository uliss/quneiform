set(SOURCES
    dummyspellchecker.cpp
    ispellchecker.cpp
    range.cpp
    spellcheckhighlighter.cpp
    spellcheckerfactory.cpp
)

if(APPLE) 
    list(APPEND SOURCES macspellchecker.cpp)
    list(APPEND SOURCES macspellchecker_private.mm) 
else()
    find_package(ASPELL)
    if(ASPELL_FOUND)
        add_definitions(-DHAVE_ASPELL)
        add_definitions(${ASPELL_DEFINITIONS})
        list(APPEND SOURCES aspellchecker.cpp)
    endif()
endif()

source_group("Source Files" FILES ${SOURCES})

add_library(quneiform_spell STATIC ${SOURCES})

set(SPELL_LIBS)
if(APPLE)
    list(APPEND SPELL_LIBS quneiform_mac)
endif()
if(ASPELL_FOUND)
    list(APPEND SPELL_LIBS ${ASPELL_LIBRARIES})
endif()

target_link_libraries(quneiform_spell ${SPELL_LIBS})



