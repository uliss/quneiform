if(CF_USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
    if(ICONV_SECOND_ARGUMENT_IS_CONST)
        add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    endif()
    set(CF_ICONV_LIBS ${ICONV_LIBRARIES})
endif()

add_library(cuneiform-common ${LIBTYPE}
    cifconfig.cpp
    helper.cpp
    iconv.cpp
    iconvimpl.cpp
    image.cpp
    imagerawdata.cpp
    language.cpp
    outputformat.cpp
)

set(COMMON_HEADER_INSTALL "common")
install(FILES 
    language.h
    exception.h
    image.h
    imagerawdata.h
    rect.h
    point.h
    size.h
    singleton.h
    outputformat.h
    cifconfig.h
    memorybuffer.h 
    DESTINATION include/${CF_HEADER_INSTALL}/${COMMON_HEADER_INSTALL})

target_link_libraries(cuneiform-common ${CF_ICONV_LIBS})

library_hook(cuneiform-common) 
