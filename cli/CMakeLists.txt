include_directories(
../src
../src/h
../src/include
../src/hhh
)

set(CF_GETOPTS "")
if(MSVC) 
    set(CF_GETOPT getopt.c getopt1.c)
endif()

add_executable(cuneiform-cli cuneiform-cli.cpp ${CF_GETOPT})

if(NOT MSVC) 
    set_target_properties(cuneiform-cli PROPERTIES OUTPUT_NAME cuneiform)
endif()

target_link_libraries(cuneiform-cli cuneiform ${cli_ext_libs} ${DL_LIB})
install_exe(cuneiform-cli)
