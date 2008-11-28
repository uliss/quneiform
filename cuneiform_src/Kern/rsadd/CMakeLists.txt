include_directories(BEFORE src)

add_definitions(-D__RSADD__)

add_library(rsadd ${LIBTYPE} src/rsadd.c)

target_link_libraries(rsadd cstr)

if(WIN32)
  install(TARGETS rsadd ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rsadd LIBRARY DESTINATION ${LIBDIR})
endif()

