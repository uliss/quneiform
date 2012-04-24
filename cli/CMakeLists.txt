include_directories(
    ../src
    ../src/h
    ../src/include
    ../src/hhh
)

set(CLI_LIBS cuneiform cuneiform-options)

set(CLI_SOURCES
    clioptions.cpp
    options.cpp
    optionsparser.cpp
)

if(MSVC)
    list(APPEND CLI_SOURCES getopt.c getopt1.c)
    include_directories(.)
endif()

add_library(cuneiform-options STATIC
    ${CLI_SOURCES}
)

add_executable(cuneiform-cli cuneiform-cli.cpp)
if(NOT MSVC) 
    set_target_properties(cuneiform-cli PROPERTIES OUTPUT_NAME cuneiform)
endif()

target_link_libraries(cuneiform-cli ${CLI_LIBS})
install_exe(cuneiform-cli)

add_executable(cf-binarize cf-binarize.cpp)
target_link_libraries(cf-binarize cuneiform-rimage cuneiform-cimage cuneiform-rdib)

if(CF_USE_SHMEM)
    add_executable(cuneiform-process cuneiform_process_cli.cpp)
    target_link_libraries(cuneiform-process ${CLI_LIBS})
endif()

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-cli UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
