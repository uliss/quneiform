add_executable(cuneiform cuneiform-cli.cpp)
target_link_libraries(cuneiform puma ${ImageMagick_Magick++_LIBRARY} ${DL_LIB})
install_exe(cuneiform)
