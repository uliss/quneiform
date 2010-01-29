include_directories(${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
   add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

add_library(cuneiform-common ${LIBTYPE}
cifconfig.cpp
helper.cpp
iconv.cpp
language.cpp
outputformat.cpp
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


target_link_libraries(cuneiform-common ${ICONV_LIBRARIES})

library_hook(cuneiform-common) 
