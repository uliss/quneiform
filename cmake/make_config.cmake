# Making config headers
configure_file(${CMAKE_SOURCE_DIR}/src/include/config.h.in ${CMAKE_BINARY_DIR}/src/include/config.h)
configure_file(${CMAKE_SOURCE_DIR}/src/include/config-user.h.in ${CMAKE_BINARY_DIR}/src/include/config-user.h)

include_directories(${CMAKE_BINARY_DIR}/src/include)