configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/generate_build_number.cmake.in"
    "${CMAKE_BINARY_DIR}/generate_build_number.cmake"
    IMMEDIATE @ONLY)

add_custom_target(update_build_number ALL
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/generate_build_number.cmake
)
