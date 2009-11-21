include_directories(
../src
../src/h
../src/include
../src/hhh
../src/hdebug
../src/hrk
../src/hrkint
../src/hrstr
../src/usage
)

add_executable(cuneiform cuneiform-cli.cpp)
target_link_libraries(cuneiform puma ${cli_ext_libs} ${DL_LIB})
install_exe(cuneiform)
