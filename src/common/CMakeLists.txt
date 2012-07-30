include_directories(BEFORE ${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
    add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

set(CF_COMMON_SOURCES
    cifconfig.cpp
    configparser.cpp
    console_messages.cpp
    consoleprogresscounter.cpp
    ctdib.cpp
    envpaths.cpp
    filesystem.cpp
    formatoptions.cpp
    helper.cpp
    iconv_local.cpp
    iconvimpl.cpp
    image.cpp
    imageformats.cpp
    imagerawdata.cpp
    imageurl.cpp
    language.cpp
    log.cpp
    modules.cpp
    outputformat.cpp
    percentcounter.cpp
    recognizeoptions.cpp
    recognitionstate.cpp
    stringbuffer.cpp
)

set(CF_COMMON_HEADERS
    align.h
    bmp.h
    consoleprogresscounter.h
    debug.h
    dib.h
    endianess.h
    language.h
    lang_def.h
    exception.h
    fontstyle.h
    formatoptions.h
    image.h
    imagerawdata.h
    imageformats.h
    imageurl.h
    interval.h
    letter.h
    modules.h
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

source_group("Header Files" FILES ${CF_COMMON_HEADERS})

add_library(${CF_COMMON} ${LIBTYPE}
    ${CF_COMMON_SOURCES}
    ${CF_COMMON_HEADERS}
)

target_link_libraries(${CF_COMMON}
    ${CF_COMPAT}
    ${ICONV_LIBRARIES}
)
  
cf_install_library(${CF_COMMON})
cf_install_headers(common ${CF_COMMON_HEADERS})
cf_hide_visibility(${CF_COMMON})
cf_cppcheck(${CF_COMMON})
