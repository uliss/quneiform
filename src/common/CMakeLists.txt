if(CF_USE_ICONV)
    include_directories(BEFORE ${ICONV_INCLUDE_DIR})
    if(ICONV_SECOND_ARGUMENT_IS_CONST)
        add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    endif()
    set(CF_ICONV_LIBRARIES ${ICONV_LIBRARIES})
endif()

add_library(cuneiform-common ${LIBTYPE}
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

set(COMMON_HEADER_INSTALL "common")
install(FILES 
    align.h
    language.h
    lang_def.h
    exception.h
    formatoptions.h
    image.h
    imagerawdata.h
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
    DESTINATION include/${CF_HEADER_INSTALL}/${COMMON_HEADER_INSTALL})

target_link_libraries(cuneiform-common ${CF_ICONV_LIBRARIES})

library_hook(cuneiform-common) 
