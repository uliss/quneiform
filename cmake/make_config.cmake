# Making config headers
configure_file(${CMAKE_SOURCE_DIR}/src/include/config.h.in ${CMAKE_BINARY_DIR}/src/include/config.h)
configure_file(${CMAKE_SOURCE_DIR}/src/include/config-user.h.in ${CMAKE_BINARY_DIR}/src/include/config-user.h)
configure_file(${CMAKE_SOURCE_DIR}/src/include/config-serialize.h.in ${CMAKE_BINARY_DIR}/src/include/config-serialize.h)
configure_file(${CMAKE_SOURCE_DIR}/src/include/config-version.h.in ${CMAKE_BINARY_DIR}/src/include/config-version.h)
configure_file(${CMAKE_SOURCE_DIR}/gui/theme-config.h.in ${CMAKE_BINARY_DIR}/gui/theme-config.h)

include_directories(${CMAKE_BINARY_DIR}/src/include)
