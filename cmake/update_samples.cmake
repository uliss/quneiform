configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/update_samples_odf.cmake.in"
    "${CMAKE_BINARY_DIR}/update_samples_odf.cmake"
    IMMEDIATE @ONLY)

add_custom_target(update_samples_odf
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/update_samples_odf.cmake)

configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/update_samples_fb2.cmake.in"
    "${CMAKE_BINARY_DIR}/update_samples_fb2.cmake"
    IMMEDIATE @ONLY)

add_custom_target(update_samples_fb2
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/update_samples_fb2.cmake)
