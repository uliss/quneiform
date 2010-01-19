include_directories(
../src
../src/h
../src/include
../src/hhh
)

add_executable(cuneiform-cli cuneiform-cli.cpp)
set_target_properties(cuneiform-cli PROPERTIES OUTPUT_NAME cuneiform)
target_link_libraries(cuneiform-cli cuneiform ${cli_ext_libs} ${DL_LIB})
install_exe(cuneiform-cli)
