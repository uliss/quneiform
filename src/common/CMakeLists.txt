include_directories(${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
   add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

add_library(cuneiform-common ${NEWLIBTYPE}
cifconfig.cpp
helper.cpp
iconv.cpp
language.cpp
)

target_link_libraries(cuneiform-common ${ICONV_LIBRARIES})

library_hook(cuneiform-common) 
