if(USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
    set(iconv_local "iconv.cpp")
    if(ICONV_SECOND_ARGUMENT_IS_CONST)
        add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    endif()
else()
    set(iconv_local "")
endif()

add_library(cuneiform-common ${LIBTYPE}
    cifconfig.cpp
    helper.cpp
    language.cpp
    outputformat.cpp
    ${iconv_local}
)

set(COMMON_HEADER_INSTALL "common")
install(FILES 
    language.h
    exception.h
    rect.h
    point.h
    size.h
    singleton.h
    outputformat.h
    cifconfig.h
    memorybuffer.h 
    DESTINATION include/${CF_HEADER_INSTALL}/${COMMON_HEADER_INSTALL})

if(USE_ICONV)
    target_link_libraries(cuneiform-common ${ICONV_LIBRARIES})
endif()

library_hook(cuneiform-common) 
