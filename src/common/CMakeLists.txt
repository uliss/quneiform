include_directories(BEFORE ${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
    add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

if(Boost_VERSION GREATER 104400)
    add_definitions(-DBOOST_FILESYSTEM_VERSION=3)
endif()

set(COMMON_SOURCES
    cifconfig.cpp
    console_messages.cpp
    consoleprogresscounter.cpp
    envpaths.cpp
    filesystem.cpp
    formatoptions.cpp
    helper.cpp
    iconv_local.cpp
    iconvimpl.cpp
    image.cpp
    imageformats.cpp
    imagerawdata.cpp
    language.cpp
    outputformat.cpp
    percentcounter.cpp
    recognizeoptions.cpp
    recognitionstate.cpp
)

set(COMMON_HEADERS
    align.h
    consoleprogresscounter.h
    endianess.h
    language.h
    lang_def.h
    exception.h
    fontstyle.h
    formatoptions.h
    image.h
    imagerawdata.h
    imageformats.h
    letter.h
    rect.h
    percentcounter.h
    point.h
    serialize.h
    size.h
    singleton.h
    outputformat.h
    recognizeoptions.h
    cifconfig.h
    memorybuffer.h
)

source_group("Header Files" FILES ${COMMON_HEADERS})

add_library(cuneiform-common ${LIBTYPE}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

cif_visibility_hidden(cuneiform-common)

target_link_libraries(cuneiform-common
    ${ICONV_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

install(FILES
    ${COMMON_HEADERS}
    DESTINATION include/${CF_HEADER_INSTALL}/common)

library_hook(cuneiform-common) 
