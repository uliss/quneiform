include_directories(BEFORE ${ICONV_INCLUDE_DIR})

add_library(common ${LIBTYPE}
cifconfig.cpp
iconv.cpp
)

target_link_libraries(common ${ICONV_LIBRARIES})

install_dll(common)
