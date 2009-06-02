add_executable(cuneiform cuneiform-cli.cpp)
target_link_libraries(cuneiform puma ${MAGICK_LIBRARIES} ${DL_LIB})
install_exe(cuneiform)
