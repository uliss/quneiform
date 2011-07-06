include_directories(BEFORE ${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
    add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

set(COMMON_SOURCES
    cifconfig.cpp
    formatoptions.cpp
    helper.cpp
    iconv_local.cpp
    iconvimpl.cpp
    image.cpp
    imagerawdata.cpp
    language.cpp
    outputformat.cpp
    recognizeoptions.cpp
)

set(COMMON_HEADERS
    align.h
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

target_link_libraries(cuneiform-common ${ICONV_LIBRARIES})

install(FILES
    ${COMMON_HEADERS}
    DESTINATION include/${CF_HEADER_INSTALL}/common)

library_hook(cuneiform-common) 
