include_directories(BEFORE ${ICONV_INCLUDE_DIR})

if(ICONV_SECOND_ARGUMENT_IS_CONST)
   add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif()

add_library(common ${LIBTYPE}
cifconfig.cpp
helper.cpp
iconv.cpp
)

target_link_libraries(common ${ICONV_LIBRARIES})

library_hook(common) 
